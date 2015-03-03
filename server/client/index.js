var React = require('react');
var Bootstrap = require('react-bootstrap');
var Backbone = require('backbone');
var Button = Bootstrap.Button;
var Modal = Bootstrap.Modal;

var DropdownStateMixin = require('react-bootstrap/DropdownStateMixin')

var Progress = require('react-progressbar');

var _ = require('underscore');

var Toolbar = React.createClass({

    handleBuild: function() {
        this.props.onBuild(false);
    },

    handleCleanBuild: function() {
        this.props.onBuild(true);
    },

    render: function() {

        var codeButtonClass = "btn btn-default";
        var settingsButtonClass = "btn btn-default";

        if (this.props.settings) {
            settingsButtonClass += " active";
        } else {
            codeButtonClass += " active";
        }

        // <div className='breadcrumbs'>Hello, world</div>
        return <div className='toolbar'>
            <div className='btn-group buttons'>
                <h1></h1>
                <a href='#' className='btn btn-default' onClick={this.handleBuild}>Build</a>
                <a href='#' className='btn btn-default' onClick={this.handleCleanBuild}>Clean Build</a>
            </div>
                <div className="btn-group">
                    <a href='#' className={codeButtonClass} onClick={this.props.onCode}><i className="fa fa-code"></i></a>
                    <a href='#' className={settingsButtonClass} onClick={this.props.onSettings}><i className='fa fa-cog'></i></a>
                </div>
                <h1> Hello <small>Connection: {this.props.connection}</small></h1>
            </div>
    }

});

var Source = React.createClass({

    getInitialState: function() {
        return {source: "exe hello : hello.cpp ;"};
    },

    render: function() {
        return <span>{this.state.source}</span>;
    }
});

var Message = React.createClass({
   render: function() {

       var sub;
       var badge;
       var klass;

       var submessages = this.props.message.submessages;
       if (submessages !== undefined && submessages.length != 0) {
           badge = <a href='#' onClick={this.handleClick}><span className='badge'>{submessages.length}</span></a>;
           var subtext = submessages[submessages.length-1];

           sub = <span className='sub'>{subtext}</span>;
       }

       var text = this.props.message.text;
       if (text === 'Done') {
           klass = 'done';
       }

       return <p className={klass}>{text} {badge} {sub}</p>
   },

   handleClick: function() {
       this.props.onClick(this.props.message);
   }
});

var DetailedMessage = React.createClass({
    render: function() {

        var subelements;

        var submessages = this.props.message.submessages;
        if (submessages !== undefined) {
            subelements = submessages.map(function(s, i) {
                return <span className='sub' key={i}>{s}</span>;
            });
        }

        return <p>
            <a className='back' href='#' onClick={this.props.onClick}><i className='fa fa-arrow-circle-o-left'/></a>
            <span className='detailed'>{this.props.message.text}</span>
            {subelements}
        </p>;
    }
});

var Messages = React.createClass({

    getInitialState: function() {
        return {};
    },

    render: function() {
        var elapsed = Math.round(this.props.elapsed  / 100);
        var progress = elapsed / 10 / 6 * 100;
        if (progress > 100) {
            progress = 100;
        }


        if (this.state.focusedMessage) {
            return <div>
                <Progress completed={progress}/>
                <div className='messages'>
                    <DetailedMessage message={this.state.focusedMessage} onClick={this.showAll}/>
                </div>
            </div>;
        } else {

            var messages = this.props.messages.map(function (m, i) {
                return <Message key={i} message={m} onClick={this.showMessage}/>;
            }, this);
            return <div>
                <Progress completed={progress}/>
                <div className='messages'>{messages}</div>
            </div>;
        }
    },

    showMessage: function(message) {
        this.state.focusedMessage = message;
        this.setState(this.state);
    },

    showAll: function() {
        this.state.focusedMessage = null;
        this.setState(this.state);
    }
});

var Typeahead = React.createClass({

    getInitialState: function() {
        return {
            menuShown: false
        }
    },

    render: function() {

        var classname;
        if (this.props.error) {
            classname = "error";
        }

        var menu;
        if (this.state.menuShown) {

            var self = this;
            var values = this.props.values.map(function(v) {
               function h() {
                    console.log("Click");
               }
               return <li key={v}><a href="#" onMouseDown={self.handleMenuClick}>{v}</a></li>
            });

            menu = <ul className="dropdown-menu" style={{display: "block"}}>{values}</ul>;
        }

        return <div className="dropdown typeAhead">
            <input type='text' value={this.props.value}  className={classname}
                   onKeyDown={this.handleKeyDown}
                   onFocus={this.handleFocus}
                   onBlur={this.handleFocusOut}
                   onClick={this.handleClick}
                   onChange={this.props.onChange}></input>
            {menu}
            </div>
    },

    handleMenuClick: function(e) {
        console.log(e.target.text);
        this.props.onChange(e.target.text);
    },

    handleFocus: function() {
        this.setState({menuShown: true});
    },

    handleFocusOut: function() {
        this.setState({menuShown: false});
    },

    handleKeyDown: function(e) {
        if (e.keyCode === 27) {
            this.setState({menuShown: false});
        }
    },

    handleClick: function() {
        this.setState({menuShown: true});
    },

    componentDidMount: function() {
        var self = this;
        this.getDOMNode().onfocus = function() {
            self.setState({menuShown: true});
        }


        //$(this.getDOMNode()).typeahead({
        //    source: this.props.values,
        //    minLength: 0,
        //    showHintOnFocus: true
        //});
    }

});

var Feature = React.createClass({

    getInitialState: function() {
        return { value: null, error: false};
    },

    handleChange: function(value) {
        //this.setState({value: e.target.value});
        this.props.onChange(this.props.feature, value);

        if (this.timer) {
            clearTimeout(this.timer);
        }
        this.timer = setTimeout(this.validate, 500);
    },

    validate: function() {
        var error;
        if (!this.state.value || this.props.feature.values.indexOf(this.state.value) != -1) {
            error = false;
        } else {
            error = true;
        }
        this.setState({error: error});
    },

   render: function() {

       var feature = this.props.feature;

       return <div className="property">
           <span>{this.props.feature.name}</span>
           <Typeahead value={feature.value} values={feature.values} onChange={this.handleChange} error={this.state.error}/>
       </div>;
   }
});

var BuildProperties = React.createClass({

    render: function() {
        var self = this;
        var features = this.props.features.map(function(f) {
            return <Feature feature={f} onChange={self.props.onChange}/>;
        });
        return <div className="properties">
                {features}
            </div>;
    }
});

var BreadcrumbsItem = React.createClass({
    mixins: [DropdownStateMixin],

    getInitialState: function() {
        return {
        }
    },

    render: function() {

        var menu;
        if (this.state.open) {

            var self = this;

            var values = this.props.values.map(function(v) {
                return <li key={v}><a href="#" onMouseDown={self.handleMenuClick.bind(self, v)}>{v}</a></li>
            });
            values = [
                <li key="<all-targets>">
                    <a href="#" onMouseDown={self.handleMenuClick.bind(self, null)}>All targets</a>
                </li>,
                <li className="divider"/>
            ].concat(values);

            menu = <ul className="dropdown-menu" style={{display: "block"}}>{values}</ul>;
        }

        var icon;
        if (this.props.type === 'project') {
            icon = <i className="fa fa-folder-o"/>;
        } else {
            icon = <i className="fa fa-dot-circle-o"/>;
        }

        return <div className="dropdown breadcrumbs-item" onKeyDown={this.handleKeyDown}>
            <span
                   onBlur={this.handleFocusOut}
                   onClick={this.handleClick}
                   onChange={this.props.onChange}>
                {icon} {this.props.value}
            </span>
            {menu}
            </div>
    },

    handleClick: function() {
        this.setDropdownState(true);
    },

    handleMenuClick: function(v) {
        this.props.onValueSelected(v);
        this.setDropdownState(false);
    }
});

var Breadcrumbs = React.createClass({

    getInitialState: function() {
        return {
            path: [
                {name: 'hello', type: 'project', children: []},
                {name: 'app', type: 'project'},
                {name: 'app'}
               // {name: 'hello', children: ['app'
            ]
        }
    },

    render: function() {

        var self = this;

        var content = [];
        var i;

        for (i = 0; i < this.props.path.length; ++i) {
            if (i > 0) {
                content.push(<i className='fa fa-chevron-right'/>);
            }
            function handler(i, v) {
                var newPath = self.props.path.slice(0, i+1);
                if (v) {
                    newPath = newPath.concat([{name: v}]);
                }
                newPath = newPath.map(function(e) { return e.name })
                console.log("newPath: " + newPath);
                this.props.onChange(newPath);
            }
            content.push(<BreadcrumbsItem
                value={this.props.path[i].name}
                type={this.props.path[i].type}
                values={this.props.path[i].children}
                onValueSelected={handler.bind(this, i)}/>)
        }

        return <div className='breadcrumbs2'>
            {content}
        </div>;
    }

});

var Settings = React.createClass({

    handleChange: function(feature, value) {
        this.props.onFeatureChange(feature, value);
    },

    handleMetatargetChange: function(path) {
        this.props.onMetatargetChange(path);
;    },

    render: function () {

        var path = [];
        var projects = [this.props.projects];
        var metatarget = this.props.metatarget;

        metatarget.forEach(function(m) {
            var p = _.find(projects, function(p) { return p.name === m;});
            var pe = {
                name: p.name
            };
            if (p.children && p.children.length) {
                pe.type = 'project';
                pe.children = _.pluck(p.children, 'name');
            }
            projects = p.children;
            path.push(pe);
        });

        return <div>
            <div className="property">
                <span style={{'vertical-align': 'middle'}}>Metatarget</span>
                <Breadcrumbs path={path} onChange={this.handleMetatargetChange}/>
            </div>
            <hr/>
            <BuildProperties features={this.props.features} onChange={this.handleChange}/>
        </div>;
    }
});

var Store = Backbone.Model.extend({
    constructor: function() {

    }
})


var BoostBuildUI = React.createClass({

    getInitialState: function() {

        this.start = new Date().getTime();
        this.step = 0;
        this.token = 1;

        this.requests = {};

        this.token2message = {};

        this.projects = {
            name: 'hello',
            type: 'project',
            children: [
                {
                    name: 'app',
                    type: 'project',
                    children: [
                        {name: 'app'}
                    ]
                },
                {
                    name: 'util/foo',
                    type: 'project',
                    children: [
                        {name: 'bar'}
                    ]
                }
            ]
        }

        this.target_path = ['hello', 'app'];

        return {elapsed: 0, messages: [], settings: false, connection: 'opening',
            projects: this.projects, target_path: this.target_path}
    },

    send: function(data) {
        this.ws.send(JSON.stringify(data));
    },

    request: function(request, callback) {
        request.token = this.token++;
        this.requests[request.token] = callback;
        this.send(request);
    },

    componentDidMount: function() {
        var self = this;
        this.ws = new WebSocket("ws://" + location.host);
        this.ws.onmessage = this.handleServerMessage;
        this.ws.onopen = function() {
            console.log("Connection is now opoen");
            self.setState({connection: 'open'});
            this.request({type: 'request', request: 'get', path: 'properties'}, function(m) {
                self.setState({features: m.response});
            });
        }.bind(this);
        this.ws.onerror = function() {
            console.log("Connection error");
            self.setState({connection: 'error'})
        }

    },

    componentWillUnmount: function() {

        clearInterval(this.timer);
        clearInterval(this.timer2);
    },

    tick: function() {
        this.state.elapsed = (new Date().getTime()) - this.start;
        this.setState(this.state);
    },

    tick2: function() {
        var last;
        if (this.state.messages.length > 0) {
            last = this.state.messages[this.state.messages.length-1];
            if (last.submessages === undefined) {
                last.submessages = [];
            }
        }

        switch(this.step) {
            case 0:
                this.state.messages.push({text: 'common.mkdir bin/gcc-4.8'});
                break;
            case 1:
                this.state.messages.push({text: 'common.mkdir bin/gcc-4.8/debug'});
                break;
            case 2:
                this.state.messages.push({text: 'gcc.compile.c++ bin/gcc-4.8/debug/hello.o'});
                break;
            case 2:
                this.state.messages.push({text: 'gcc.compile.c++ bin/gcc-4.8/debug/hello.o'});
                break;
            case 3:
                last.submessages.push("Working hard on compilation");
                break;
            case 4:
                last.submessages.push("Working some more");
                break;
            case 5:
                this.state.messages.push({text: 'gcc.link bin/gcc-4.8/debug/hello'});
                break;
            case 6:
                last.submessages.push("Linking C++ binaries is hard");
                break;
            case 7:
                this.state.messages.push({text: "Done"});
            default:
                clearInterval(this.timer2);
        }
        ++this.step;
        this.setState(this.state);
    },

    handleBuild: function(force) {

        var r = {type: 'request', request: 'build'};
        if (force) {
            r.force = true;
        }

        var properties;
        // Add any specified values of features.
        this.state.features.forEach(function(f) {
           if (f.value) {
                if (!properties) {
                    properties = {}
                }
               properties[f.name] = f.value;
           }
        });
        if (properties) {
            console.log("Build properties: " + JSON.stringify(properties));
            r.properties = properties;
        }


        if (this.state.target_path.length > 1) {

            var names = this.state.target_path.concat();
            names[0] = '.'
            var length = names.length;
            var metatarget = names.slice(0, length-1).join('/') + '//' + names[length-1];
            r.metatargets = [metatarget];
        }
        console.log("Metatargets requested: " + JSON.stringify(r.metatargets));

        this.send(r);

        this.start = new Date().getTime();
        this.step = 0;
        this.state.messages = []
        this.setState(this.state);
        clearInterval(this.timer);
        clearInterval(this.timer2)
        this.timer = setInterval(this.tick, 50);
        this.timer2 = setInterval(this.tick2, 1000);

    },

    handleCode: function() {
        this.setState({settings: false});
    },

    handleSettings: function() {
        this.state.settings = true;
        this.setState(this.state);
    },

    handleFeatureValueChange: function(feature, value) {
        feature.value = value;
        this.setState({features: this.state.features});
    },

    handleMetatargetChange: function(path) {
        this.setState({target_path: path});
    },

    handleServerMessage: function(xm) {
        var m = JSON.parse(xm.data);
        if (m.type === 'request') {
            // Quite impossible.
        } else if (m.type === 'response') {
            var token = m.token;
            if (token && this.requests[token]) {
                this.requests[token](m);
            }

        } if (m.type === 'event') {

            var text;
            if (m.event === 'build-action-started') {
                text = m['action-name'];
                if (m['targets']) {
                    text += ' ';
                    text += m['targets'][0];
                }
                m.text = text;
                m.submessages = [];
                this.token2message[m['token']] = m;
                this.state.messages.push(m);
            } else if (m.event === 'build-action-output') {
                text = m.output;
                var token = m['token'];
                if (token in this.token2message) {
                    this.token2message[token].submessages.push({text: text});
                }
            } else if (m.event === 'build-action-finished') {
                token = m['token'];
                if (token in this.token2message) {
                    this.token2message[token].success = m['exit-status'] == '0';
                }
            }
            this.setState({messages: this.state.messages});
        }
    },

    render: function() {
        var content;
        if (this.state.settings) {
            content = <Settings
                features={this.state.features} onFeatureChange={this.handleFeatureValueChange}
                projects={this.state.projects}
                metatarget={this.state.target_path} onMetatargetChange={this.handleMetatargetChange}
                />;
        } else {
            content = <Source/>;
        }
        return <div>
            <Toolbar settings={this.state.settings}
                     onBuild={this.handleBuild}
                     onCode={this.handleCode}
                     onSettings={this.handleSettings}
                     connection={this.state.connection}/>
            <div className='source'>
            {content}
            </div>
            <Messages messages={this.state.messages} elapsed={this.state.elapsed}/>
            </div>
        ;
    }
});

React.render(
    <BoostBuildUI />,
    document.getElementById('container')
);

