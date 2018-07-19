import React from 'react';
import {ipcRenderer} from 'electron';
import VelocityComponent from 'velocity-react/velocity-component';
import VelocityTransitionGroup from 'velocity-react/velocity-transition-group';


class Navbar extends React.Component{
    constructor(props){
        super(props);
        this.state = {
            isMaxed: false,
            isDrawerOpen: false
        };
        ipcRenderer.on('maxed', (event, message) =>{
            this.setState({isMaxed: message});
        });
        this.handleMini = this.handleMini.bind(this);
        this.handleMax = this.handleMax.bind(this);
        this.handleClose = this.handleClose.bind(this);
    }

    handleMini(){
        ipcRenderer.send('asynchronous-message', 'mini');
    }

    handleMax(){
        this.setState({isMaxed: ipcRenderer.sendSync('synchronous-message', 'max')});
    }

    handleClose(){
        ipcRenderer.send('asynchronous-message', 'exit');
    }

    render(){
        var drawerAnimation;
        var overlayEntry = {
            animation: {
                opacity: 0.3
            }
        };
        var overlayExit = {
            animation: {
                opacity: 0
            }
        };
        if(!this.state.isDrawerOpen){
            drawerAnimation = {
                left: "-295px"
            };
        }
        else{
            drawerAnimation = {
                left: "0px"
            };
        }

        return (
            <nav className="navbar is-fixed-top" role="navigation">
                <VelocityComponent animation={drawerAnimation} duration={250}>
                    <div className="drawer">
                        <p>Hello World!</p>
                    </div>
                </VelocityComponent>
                <VelocityTransitionGroup enter={overlayEntry} leave={overlayExit} duration={250}>
                    {this.state.isDrawerOpen ? <div id="overlay" onClick={() => this.setState({isDrawerOpen: !this.state.isDrawerOpen})}></div> : undefined}
                </VelocityTransitionGroup>
                <div id="window-bar">
                    <a id="min" className="icon is-medium" onClick={this.handleMini}>
                        <i className="mdi mdi-18px mdi-minus"></i>
                    </a>
                    <a id="max" className="icon is-medium" onClick={this.handleMax}>
                        <i className={"mdi mdi-18px " + (this.state.isMaxed ? "mdi-window-restore" : "mdi-square-outline")}></i>
                    </a>
                    <a id="close" className="icon is-medium" onClick={this.handleClose}>
                        <i className="mdi mdi-18px mdi-close"></i>
                    </a>
                </div>
                <div className="navbar-brand">
                    <div className="navbar-start">
                        <a className="button" onClick={() => this.setState({isDrawerOpen: !this.state.isDrawerOpen})}>
                            <span className="icon is-medium">
                                <i className="mdi mdi-24px mdi-menu"></i>
                            </span>
                        </a>
                        <h4 className="title is-4">Music App</h4>
                        <button onClick={this.props.addMusic}>Add Folder(s)</button>
                    </div>
                    <div className="navbar-end">
                        <div className="navbar-item">
                            <div className="control has-icons-left">
                                <input className="input" type="text" placeholder="Search" />
                                <span className="icon is-small is-left">
                                    <i className="mdi mdi-24px mdi-magnify"></i>
                                </span>
                            </div>
                        </div>
                    </div>
                </div>
                <div className="tabs">
                    <ul>
                        <li className={this.props.music.route[0].indexOf('Album') != -1 ? "is-active" : ""}>
                            <a onClick={() => this.props.navigate("allAlbums", this.props.music.route[1], this.props.music.route[2])}>Albums</a>
                        </li>
                        <li className={this.props.music.route[0].indexOf('song') != -1 ? "is-active" : ""}>
                            <a onClick={() => this.props.navigate("songs", this.props.music.route[1], this.props.music.route[2])}>Songs</a>
                        </li>
                        <li className={this.props.music.route[0].indexOf('artist') != -1 ? "is-active" : ""}>
                            <a onClick={() => this.props.navigate("artists", this.props.music.route[1], this.props.music.route[2])}>Artists</a>
                        </li>
                        <li className={this.props.music.route[0].indexOf('playlist') != -1 ? "is-active" : ""}>
                            <a onClick={() => this.props.navigate("playlists", this.props.music.route[1], this.props.music.route[2])}>Playlists</a>
                        </li>
                        <li style={{borderLeft: '1px solid #ddd',margin: '0 5px',padding: '0 5px'}}>
                            <span>Genres: </span>
                            <div className="select is-small">
                                <select>
                                    <option>All</option>
                                    <option>Blues</option>
                                    <option>Country</option>
                                    <option>Electronic</option>
                                    <option>Electro Swing</option>
                                    <option>Funk</option>
                                    <option>Hip-Hop</option>
                                    <option>Jazz</option>
                                    <option>Rock</option>
                                    <option>R&amp;B</option>
                                </select>
                            </div>
                        </li>
                        <li style={{borderLeft: '1px solid #ddd',margin: '0 5px',padding: '0 5px'}}>
                            <span>Sort by: </span>
                            <div className="select is-small">
                                <select>
                                    <option>Alphabetical</option>
                                    <option>Date Added</option>
                                    <option>Release year</option>
                                    <option>Artist</option>
                                </select>
                            </div>
                        </li>
                    </ul>
                </div>
            </nav>
        );
    }
}

export default Navbar;