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
                <h1></h1>
            </div>;

            /* <h1> Hello <small>Connection: {this.props.connection}</small></h1> */
    }

});

var Source = React.createClass({

    getInitialState: function() {
        return {source: "exe app : app.cpp /library-example/foo//bar\n        : <conditional>@add-64-bit-properties ;"};
    },

    render: function() {
        return <span className='source'>{this.state.source}</span>;
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

var PropertyGroup = React.createClass({
    render: function() {

        var self = this;
        var properties = []
        this.props.group.properties.forEach(function(f) {
            if (f.applicable === false)
                return;

            properties.push(<Feature feature={f} onChange={self.props.onChange}/>);
        });

        return <div className='property-group'>
            <h1>{this.props.group.name}</h1>
            {properties}
            </div>
    }
});

var BuildProperties = React.createClass({

    render: function() {
        var self = this;
        var groups = this.props.features.map(function(f) {
            return <PropertyGroup group={f} onChange={self.props.onChange}/>;
        });
        return <div className="properties">
                {groups}
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
        if (this.props.icon) {
            icon = <i className={'fa ' + this.props.icon}/>;
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
                this.props.onChange(newPath);
            }
            content.push(<BreadcrumbsItem
                value={this.props.path[i].name}
                values={this.props.path[i].children}
                icon={this.props.path[i].icon}
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

    handleMetaChange: function(path) {
        this.props.onMetatargetPathChange(path);
;    },

    render: function () {

        var path = this.props.metatargetPath.map(function(p) {
            var r = {
                name: p.name
            };
            if (p.type === 'project') {
                r.icon = 'fa-folder-o';
            } else {
                r.icon = 'fa-dot-circle-o';
            }
            if (p.children && p.children.length) {
                r.children = _.pluck(p.children, 'name');
            }
            return r;
        });

        return <div>
            <div className="property">
                <span style={{'vertical-align': 'middle'}}>Metatarget</span>
                <Breadcrumbs path={path} onChange={this.props.onMetatargetPathChange}/>
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

var BuildActionMessage = React.createClass({
    render: function() {

        var self = this;

        var e = this.props.event;
        var text = e['action-name'];
        if (e.targets)
            text += ' ' + e.targets[0];

        var titleClass = "action-title";
        /* Temporary disabled - yellow looks ugly really.
        if (e.errors) {
            titleClass += " error";
        } else if (e.warnings) {
            titleClass += " warning";
        } */

        var badges = [];

        if (e.lines) {
            badges.push(<span key='b-m' className='badge'>{e.lines}</span>);
        }

        if (e.warnings) {
            badges.push(<span key='b-w' className='badge bg-warning'>{e.warnings}</span>);
        }

        if (e.errors) {
            badges.push(<span key='b-e' className='badge bg-danger'>{e.errors}</span>);
        }

        if (this.props.full) {

            var submessages = this.props.event.submessages.map(function(m) {
                return <span className='command-output'>{m.text}</span>;
            });

            var commands = this.props.event.commands.map(function(c) {
                return <span className='command'>{c}</span>;
            });

            var includes;
            if (e.properties.include) {
                includes = <span>Include: {e.properties.include.join(' ')}</span>;
            }

            function onBackHere() {
                self.props.onSelection(e.parent);
            }

            return <div className='message'>
                <div className={titleClass}>
                    <a className='back' href='#' onClick={onBackHere}><i className='fa fa-arrow-circle-o-left'/></a>
                    <span className='detailed'> {text}</span> {badges}
                </div>
                <div>
                <span>Sources: {e.sources.join(' ') || 'none'}</span>
                </div>
                <div>
                    <span>Targets: {e.targets.join(' ') || 'none'}</span>
                </div>
                <div>
                    {includes}
                    </div>
                <hr/>
                <div>
                    {commands}
                </div>
                {submessages}
            </div>
        } else {

            function onDetailsHere() {
                self.props.onSelection(e);
            }

            return <div>
                <div className={titleClass} >
                    <a href='#' onClick={onDetailsHere}>{text} {badges}</a>
                </div>
                </div>
        }
    }
});


var BuildFinishedMessage = React.createClass({
    render: function() {
        var e = this.props.event;
        if (e.success === 'true') {
            return <div className="build-finished text-success">Built successfully</div>;
        } else {
            return <div className="build-finished text-danger">Build failed</div>;
        }
    }
});

var Message = React.createClass({
    render: function() {
        var e = this.props.event;
        var className = "message";
        if (e.kind == 'phase') {
            className += ' phase';
        }
        if (e.kind === 'error') {
            className += ' text-danger';
        } else if (e.kind === 'warning') {
            className += ' text-information';
        }

        var self = this;

        if (this.props.full) {

            var subs = e.submessages.map(function(s) {
                return createEventView(s, e, false, self.props.onSelection);
            });

            function onBackHere() {
                self.props.onSelection(e.parent);
            }

            return <div>
                <div className={className}>
                    <a className='back' href='#' onClick={onBackHere}><i className='fa fa-arrow-circle-o-left'/></a>
                    <span className='detailed'>{e.message}</span>
                </div>
                {subs}
            </div>;

        } else {
            var text;
            if (e.submessages && e.submessages.length > 0) {

                function onDetailsHere() {
                    self.props.onSelection(e);
                }

                text = <a href='#' onClick={onDetailsHere}>{e.message + '\u2026'}</a>;
            } else {
                text = e.message;
            }

            return <div className={className}>{text}</div>;
        }
    },

    handleDetails: function(e) {
        this.props.onDetails(e);
    }
});

function createEventView(e, p, full, handleSelection) {

    if (e.event === "build-action-started") {
        return <BuildActionMessage key={e.token} event={e} full={full}
                                   onSelection={handleSelection}/>;
    } else if (e.event === 'build-finished') {
        return <BuildFinishedMessage event={e}/>;
    } else if (e.event === 'message') {
        return <Message key={e.token} event={e} full={full} onSelection={handleSelection}/>;
    }
}

var Messages2 = React.createClass({

    getInitialState: function() {
        return {
            selection: null
        }
    },

    handleSelection: function(e) {
        this.setState({selection: e})
    },

    render: function() {

        var self = this;

        var messages;
        if (this.state.selection) {
            messages = [createEventView(this.state.selection, null, true, self.handleSelection)];
        } else {
            messages = this.props.messages.map(function (e) {
                return createEventView(e, null, false, self.handleSelection);
            });
        }

        return <div className='messages'>{messages}</div>;
    }

});



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

        this.metatarget_path = this.computeMetatargetPath(['hello', 'app']);

        return {elapsed: 0, messages: [], events: [], settings: false, connection: 'opening',
            projects: this.projects, metatarget_path: this.metatarget_path}
    },

    /* Given a list of strings, return target path consisting of
       full target objects. */
    computeMetatargetPath: function(names) {

        var result = [];
        var projects = [this.projects];
        names.forEach(function(n) {
            var p = _.find(projects, function(p) { return p.name === n;});
            result.push(p);
            projects = p.children;
        });
        return result;
    },

    send: function(data) {
        this.ws.send(JSON.stringify(data));
    },

    request: function(request, callback) {
        request.token = this.token++;
        request.type = 'request';
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
                self.flattenProperties(m.response);
                self.setState({features: m.response});
            });
        }.bind(this);
        this.ws.onerror = function() {
            console.log("Connection error");
            self.setState({connection: 'error'})
        }

    },

    componentWillUnmount: function() {

    },

    flattenProperties: function(groups) {

        var self = this;
        this.feature_map = {}
        this.feature_list = []

        var result = {}

        groups.forEach(function(g) {
            g.properties.forEach(function(p) {
                self.feature_list.push(p);
                self.feature_map[p.name] = p;
            });

            // TODO: handle children.
        });
    },

    handleBuild: function(force) {

        var r = {type: 'request', request: 'build'};
        //if (force) {
        //    r.force = true;
        //}
        r.redo = true;
        //r.noexec = true;

        var properties;
        this.feature_list.forEach(function(f) {
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

        if (this.state.metatarget_path.length > 1) {

            var names = _.pluck(this.state.metatarget_path, 'name');
            var length = names.length;
            names[0] = '.'
            if (this.state.metatarget_path[length-1].type !== 'project') {
                names[length-1] = '/' + names[length-1];
            }
            r.metatargets = [names.join('/')];
        }
        console.log("Metatargets requested: " + JSON.stringify(r.metatargets));

        this.send(r);

        this.start = new Date().getTime();
        this.step = 0;
        this.state.messages = []
        this.setState(this.state);
    },

    handleCode: function() {
        this.setState({settings: false});
    },

    handleSettings: function() {
        this.state.settings = true;
        this.setState(this.state);
    },

    handleFeatureValueChange: function(feature, value) {

        var self = this;

        feature.value = value;

        function handleApplicability(m) {
            _.each(m.response, function(value, key) {
                self.feature_map[key].applicable = value;
            }, this);
            self.setState({feature: self.state.features});
        }

        var properties = {}
        this.feature_list.forEach(function(f) {
            if (f.value) {
                if (!properties) {
                    properties = {}
                }
                properties[f.name] = f.value;
            }
        });

        this.request({
            'request': 'get-applicability',
            'properties': properties
        }, handleApplicability);

        this.setState({features: this.state.features});
    },

    handleMetatargetPathChange: function(path) {
        this.setState({metatarget_path: this.computeMetatargetPath(path)});
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

            var token = m['token'];
            /* For now, allow new message to override earlier ones.
               We'd need to consider that in future. */
            if (token) {
                this.token2message[token] = m;
            }

            var text;
            if (m.event === 'build-action-started') {
                text = m['action-name'];
                if (m['targets']) {
                    text += ' ';
                    text += m['targets'][0];
                }
                m.text = text;
                m.submessages = [];
                m.lines = 0;
                m.warnings = 0;
                m.errors = 0;

                var p = m['parent'];
                if (p) {
                    var parent = this.token2message[p];
                    m.parent = parent;
                    parent.submessages.push(m);
                } else {
                    this.state.messages.push(m);
                }
            } else if (m.event === 'build-action-output') {
                text = m.output;
                var parent = m['parent'];
                if (parent in this.token2message) {
                    var parent = this.token2message[parent];
                    m.parent = parent;
                    parent.submessages.push({text: text});
                    parent.lines++;
                    if (m['output-kind'] === 'warning') {
                        parent.warnings++;
                    } else if (m['output-kind'] === 'error') {
                        parent.errors++;
                    }
                }
            } else if (m.event === 'build-action-finished') {
                token = m['token'];
                if (token in this.token2message) {
                    this.token2message[token].success = m['exit-status'] == '0';
                }
            } else if (m.event === 'build-finished') {
                this.state.messages.push(m);
            } else if (m.event === 'message') {

                var p = m['parent'];
                if (m.kind == 'configuration-check-result') {
                    this.token2message[p].message += ": " + m.message;
                } else {
                    if (p) {
                        var parent = this.token2message[p];
                        m.parent = parent;
                        parent.submessages.push(m);
                    } else {
                        m.submessages = [];
                        this.state.messages.push(m);
                    }
                }
            }
            this.setState({
                messages: this.state.messages
            });
        }
    },

    render: function() {
        var content;
        if (this.state.settings) {
            content = <Settings
                features={this.state.features} onFeatureChange={this.handleFeatureValueChange}
                projects={this.state.projects}
                metatargetPath={this.state.metatarget_path} onMetatargetPathChange={this.handleMetatargetPathChange}
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
            <Messages2 messages={this.state.messages}/>
        </div>
            ;
    }
});

React.render(
    <BoostBuildUI />,
    document.getElementById('container')
);

