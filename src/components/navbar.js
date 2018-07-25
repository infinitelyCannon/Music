import React from 'react';
import VelocityComponent from 'velocity-react/velocity-component';
import VelocityTransitionGroup from 'velocity-react/velocity-transition-group';

class Navbar extends React.Component{
    constructor(props){
        super(props);
        this.state = {
            isDrawerOpen: false,
            worker: null
        };
        this.handleGenre = this.handleGenre.bind(this);
        this.handleSort = this.handleSort.bind(this);
    }

    handleGenre(e){
        this.props.onNavClick("filter", e.target.value);
    }

    handleSort(e){
        this.props.onNavClick("sort", {type: e.target.value, ascend: true});
    }

    scanFiles(paths){
        if(this.state.worker === null){
            this.setState({worker: new Worker('./dist/worker.js')});
            this.state.worker.onmessage = function(e){
                this.props.importFolders(e.data);
                this.state.worker.terminate();
                this.setState({worker: null});
            }.bind(this);
            this.state.worker.postMessage([paths, window.dataPath]);
        }
    }

    openFolders(){
        window.dialog.showOpenDialog({
            title: "Select your folder(s)",
            properties: ["openDirectory"]
        }, (folderPaths) => {
            if(folderPaths === undefined){
                console.info("No destination folder selected");
                return;
            }
            else{
                this.scanFiles(folderPaths);
                return;
            }
        });
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
            <nav className="navbar is-fixed-top">
                <VelocityComponent animation={drawerAnimation} duration={250}>
                    <div className="drawer">
                        <p>Hello World!</p>
                    </div>
                </VelocityComponent>
                <VelocityTransitionGroup enter={overlayEntry} leave={overlayExit} duration={250}>
                    {this.state.isDrawerOpen ? <div id="overlay" onClick={() => this.setState({isDrawerOpen: !this.state.isDrawerOpen})}></div> : undefined}
                </VelocityTransitionGroup>
                <div className="navbar-brand">
                    <div className="navbar-start">
                        <a className="button" onClick={() => this.setState({isDrawerOpen: !this.state.isDrawerOpen})}>
                            <span className="icon is-medium">
                                <i className="mdi mdi-24px mdi-menu"></i>
                            </span>
                        </a>
                        <h4 className="title is-4">Music App</h4>
                        <button onClick={() => this.openFolders()}>Add Folder(s)</button>
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
                        <li className={this.props.route.view === "albums" ? "is-active" : ""}>
                            <a onClick={() => this.props.onNavClick("view", "albums")}>Albums</a>
                        </li>
                        <li className={this.props.route.view === "songs" ? "is-active" : ""}>
                            <a onClick={() => this.props.onNavClick("view", "songs")}>Songs</a>
                        </li>
                        <li className={this.props.route.view === "artists" ? "is-active" : ""}>
                            <a onClick={() => this.props.onNavClick("view", "artists")}>Artists</a>
                        </li>
                        <li className={this.props.route.view === "playlists" ? "is-active" : ""}>
                            <a onClick={() => this.props.onNavClick("view", "playlists")}>Playlists</a>
                        </li>
                        <li style={{borderLeft: '1px solid #ddd',margin: '0 5px',padding: '0 5px'}}>
                            <span>Genres: </span>
                            <div className="select is-small">
                                <select onChange={(e) => this.handleGenre(e)}>
                                    <option>All</option>
                                    {
                                        this.props.genres.map((item) => {
                                            if(item != "All"){
                                                return <option key={item}>{item}</option>
                                            }
                                        })
                                    }
                                </select>
                            </div>
                        </li>
                        <li style={{borderLeft: '1px solid #ddd',margin: '0 5px',padding: '0 5px'}}>
                            <span>Sort by: </span>
                            <div className="select is-small">
                                <select onChange={(e) => this.handleSort(e)}>
                                    <option value="title">Alphabetical</option>
                                    <option value="dateAdded">Date Added</option>
                                    <option value="year">Release year</option>
                                    <option value="artist">Artist</option>
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