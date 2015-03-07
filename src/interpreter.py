import re
import os
import tempfile
import subprocess
import shutil
import sys


RULE_DECL_REGEX = re.compile(r'\s*rule\s+(?P<name>[\w\d_-]+)(\s+\( .*? \))?')
MODULE_DECL_REGEX = re.compile(r'\s*module\s+(?P<name>[\w\d_-]+)\s+')
VARIABLE_DECL_REGEX = re.compile(r'\s*(local\s+[\w\d_-]+)|([\w\d_-]+\s+\=)')
IMPORT_DECL_REGEX = re.compile(r'\s*import .*?;')

EVAL_VARIABLE_REGEX = re.compile(r'\s*\$\(.+?\)')
EVAL_RULE_REGEX = re.compile(r'\s*\[ .*? \]')

FOR_REGEX = re.compile(r'\s*for\s+(local\s+)?(?P<var>[\w\d_-]+)\s+in\s+(.*?)\{?')
WHILE_REGEX = re.compile(r'\s*while\s+.*?\{?')
IF_REGEX = re.compile(r'\s*if\s+(?P<condition>.*?)\{?')
ELSE_REGEX = re.compile(r'\s*else\s+(if\s+(?P<condition>.*?))?\{?')
SWITCH_REGEX = re.compile(r'\s*switch\s+(?P<value>.*?)\{?')

has_body_regexes = [RULE_DECL_REGEX, MODULE_DECL_REGEX,
                    FOR_REGEX, WHILE_REGEX, IF_REGEX, ELSE_REGEX, SWITCH_REGEX]

should_save_regexes = [RULE_DECL_REGEX, MODULE_DECL_REGEX,
                       VARIABLE_DECL_REGEX, IMPORT_DECL_REGEX]


class InteractiveShell(object):
    def __init__(self, b2_exe='b2', boost_build_path=''):
        self.line = ''
        self.paren_count = 0
        self.brace_count = 0
        self.quote_count = 0
        self.block_count = 0

        self.char_map = {
            '(': self.paren_increase,
            ')': self.paren_decrease,
            '{': self.brace_increase,
            '}': self.brace_decrease,
            '"': self.quote_increase,
        }
        self.commands = []
        self.saved_commands = []
        self.seen_semi_colon = True
        self.exe = b2_exe
        if boost_build_path:
            os.environ['BOOST_BUILD_PATH'] = os.pathsep.join(boost_build_path)
        self.tempdir = os.path.join(tempfile.gettempdir(), 'temp-interactive-jam')
        if not os.path.exists(self.tempdir):
            os.mkdir(self.tempdir)
        self.tempfile = os.path.join(self.tempdir, 'Jamroot.jam')
        self.saved_cwd = os.getcwd()
        self.looking_for_else = False
        self.been_inside_body = False
        self.found_else = False
        os.chdir(self.tempdir)

    def paren_increase(self):
        self.paren_count += 1

    def brace_increase(self):
        self.brace_count += 1

    def paren_decrease(self):
        self.paren_count -= 1

    def brace_decrease(self):
        self.brace_count -= 1
        self.block_count += 1

    def quote_increase(self):
        self.quote_count += 1

    def noop(self):
        pass

    def should_save(self):
        """
        Since this interpreter is so stupidly simple, it has
        to keep track of statements that preserve state.
        Statements such as variable declarations, imports,
        rules, etc.
        This saves all of those statements for further output
        to the temp file.
        """
        commands = '\n'.join(self.commands).strip()
        should_save = False
        for regex in should_save_regexes:
            should_save = should_save or bool(regex.match(commands))
            if should_save:
                return True
        return False

    def autoecho(self):
        """
        To make evaluating variables and rules a little bit easier,
        this allows specifying just a variable name or calling
        a rule that returns something and will automatically
        echo the values.
        """
        lines_str = ' '.join(self.commands)
        if EVAL_VARIABLE_REGEX.match(lines_str) or EVAL_RULE_REGEX.match(lines_str):
            self.commands.insert(0, 'echo')


    def send_command(self):
        """
        Now that we have a complete command write all of the previously saved
        commands as well as the new command to the temp Jamroot and run b2
        on it. Capture the output and display it. If the return code is
        non-zero and the command is "saveable" then the command is *not*
        saved since that would cause calling the Jamroot multiple times
        to error out thereafter.
        """
        self.autoecho()
        file_contents = '\n'.join(self.saved_commands + self.commands) + '\nexit : 0 ;'
        with open(self.tempfile, 'w') as f:
            f.write(file_contents)
        p = subprocess.Popen([self.exe, '--python', '--stacktrace'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = p.communicate()
        if stdout.strip():
            lines = []
            for line in stdout.splitlines():
                # remove the experimental banner
                if not 'Boost.Build V2 Python port (experimental)' in line:
                    lines.append(line)
            lines = '\n'.join(lines)
            if lines:
                if not lines.endswith('\n'):
                    lines += '\n'
                sys.stdout.write(lines)
        if stderr.strip():
            print stderr + '\n'
        if not p.returncode and self.should_save():
            self.saved_commands.extend(self.commands)
        self.commands = []

    @property
    def in_quotes(self):
        """
        Is parsing currently in the middle of a quote (this means that the
        quote is spanning multiple lines)?
        """
        return self.quote_count % 2 != 0

    def is_body_statement(self, line):
        """
        Is the contents of "line" the beginning of a Jam element that
        has a body?
        """
        result = False
        for regex in has_body_regexes:
            result = bool(regex.match(line))
            if result:
                break
        return result

    def keep_going(self):
        """
        Returns True if the parser should continue to consume lines before
        handing it off to b2 to be evaluated.
        """
        command_str = self.commands[0].strip() if self.commands else ''
        if self.is_body_statement(command_str):
            keep_going = (self.paren_count != 0
                          or self.quote_count % 2 != 0
                          or self.brace_count != 0
                          or self.block_count < 1)
            if self.brace_count != 0:
                self.inside_body = True
            elif self.brace_count == 0 and self.block_count > 0:
                self.inside_body = False
                self.been_inside_body = True

            if IF_REGEX.match(command_str) and not keep_going and self.been_inside_body and not self.found_else:
                # the if statement also allows for else blocks to follow,
                # so this tells the interpreter to keep going in case an else
                # follows. This resets some of the state machine.
                keep_going = True
                self.looking_for_else = True
                self.inside_body = False
                self.been_inside_body = False
                self.block_count = 0
                self.found_else = False
            elif self.looking_for_else:
                if ELSE_REGEX.match(self.commands[-1]):
                    self.found_else = True
                    self.looking_for_else = False
                else:
                    keep_going = False
        else:
            keep_going = not self.seen_semi_colon

        return keep_going

    def next_line(self):
        """
        Get the next line from user input. If the command spreads over
        multiple lines, the parser will keep consuming until a semicolon
        of the body of the element is completed. Then it hands off the
        full command to b2 in the call to send_commands().
        """
        if self.keep_going():
            return raw_input('... ')
        if self.commands:
            self.send_command()
        # reset the state machine
        self.quote_count = 0
        self.block_count = 0
        self.seen_semi_colon = False
        self.looking_for_else = False
        self.found_else = False
        self.inside_body = False
        self.been_inside_body = False
        return raw_input('>>> ')

    def parse_line(self, line):
        """
        Keep track of brace, paren, quote, and block statistics. If a
        semicolon is seen, everything up to and including the semicolon
        is returned as a "complete" command and then the rest of the
        line is returned as "leftover".
        """
        if not self.in_quotes:
            line = line.strip()
        if not line:
            # treat it like a newline
            # like the python interpreter.
            self.seen_semi_colon = True
            return '', ''
        idx = 0
        for idx, c in enumerate(line):
            # if we're inside of a quote here, then we don't want to
            # update the statistics because the parens, braces, etc
            # are just literals within the string.
            if not self.in_quotes:
                self.char_map.get(c, self.noop)()
                if c == ';':
                    self.seen_semi_colon = True
                    idx += 1
                    break
                # ignore comments
                elif c == '#':
                    # everything from this character to
                    # the end of the line is a comment;
                    # thus, there will not be any leftover
                    line = line[:idx]
                    break
            else:
                # we still have to keep track of quotes, though.
                # that way we can continue counting the other characters
                if c == '"':
                    self.quote_increase()
        # is there any leftovers?
        if len(line) - 1 == idx:
            return line, ''
        # there is some leftovers, return everything up to and including the
        # semicolon as well as the rest of the line which will be
        # re-evaluated on the next pass.
        return line[:idx], line[idx:]

    def _interact_really(self):
        self.line = line = self.next_line()
        while line:
            line, leftover = self.parse_line(line)
            if line.lower().strip() == 'exit ;':
                return False
            elif line.lower().strip() == 'help ;':
                self.print_help()
                return True
            self.commands.append(line)
            line = leftover
        return True

    def interact(self):
        print 'Boost.Build Interactive Shell'
        print 'Type "help ;" for a simple tutorial.'
        print '_' * 80
        try:
            while self._interact_really(): pass
        finally:
            os.chdir(self.saved_cwd)
            shutil.rmtree(self.tempdir, ignore_errors=True)

    def print_help(self):
        from textwrap import dedent
        print dedent("""
            Welcome to the Boost.Build Interactive Jam Interpreter!
            For more information on the Jam language, check out the guide
            on the Boost.Build documentation:
            http://www.boost.org/build/doc/html/jam/language.html

            To exit the shell, just type:
            >>> exit ;

            This interactive shell is a playground for the Jam language.
            Use it to test out some of the language constructs. For example:

            >>> # To create a variable:
            >>> my-variable = hello world ;
            >>> # Then, to evaluate the variable, simply type its name:
            >>> $(my-variable) ;
            hello world
            >>> # You can create rules:
            >>> rule say-hello ( name ? )
            ... {
            ...     name ?= World ;
            ...     echo Hello $(name)! ;
            ... }
            >>> # And then evaluate them:
            >>> say-hello Aaron ;
            Hello Aaron!
            >>> # Modules can also be imported:
            >>> import regex ;
            >>> # And rules that return values can also
            >>> # be easily evaluated:
            >>> sources = source1.c source2.c source3.c ;
            >>> sources = prefix/$(sources) ;
            >>> [ regex.replace-list $(sources) : prefix/ : new-prefix ] ;
            new-prefix/source1.c new-prefix/source2.c new-prefix/source3.c
        """)


if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument('-e', '--exe', default='b2',
                        help='path/to/b2; default expects b2 to be '
                             'in your path')
    parser.add_argument('-p', '--boost-build-path', default='',
                        help='a "," separated path for all '
                             'modules that should be made importable. '
                             'Defaults to BOOST_BUILD_PATH environment '
                             'variable.')
    args = parser.parse_args()

    path = args.boost_build_path.split(',')
    shell = InteractiveShell(args.exe, path)
    shell.interact()
