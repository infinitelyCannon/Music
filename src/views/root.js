import React from 'react';
import _ from 'underscore';
require('velocity-animate');
require('velocity-animate/velocity.ui');
import NavBar from './Navbar';
import BottomNav from './BottomNav';
//import Content from './Content';
//const fs = require('fs');

class RootView extends React.Component{
    constructor(props){
        super(props);
        this.openFolders = this.openFolders.bind(this);
        this.state = {
            worker: null
        };
    }

    openFolders(){
        window.dialog.showOpenDialog({
            title: "Select a folder",
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

    scanFiles(paths){
        if(this.state.worker == null){
            this.setState({worker: new Worker('./dist/worker.js')});
            this.state.worker.onmessage = function(e){
                if(e.data === 0){
                    this.state.worker.terminate();
                    this.setState({worker: null});
                }
                else{
                    this.props.onAdd(e.data);
                }
            }.bind(this);
            this.state.worker.postMessage(paths);
        }
    }

    render(){
        return (
            <div>
                <NavBar />
                <div id="container" style={{paddingTop: '20px'}}>
                    <button style={{height: 'fit-content'}} onClick={this.openFolders}>Add Folder(s)</button>
                    <table className="table is-fullwidth is-striped is-hoverable">
                        <thead>
                            <tr>
                                <th>Name</th>
                                <th>Artist</th>
                                <th>Album</th>
                                <th>
                                    <span className="icon is-medium">
                                        <i className="mdi mdi-24px mdi-clock-outline"></i>
                                    </span>
                                </th>
                            </tr>
                        </thead>
                        <tbody>
                            {
                                [...this.props.songs.values()].map((val) => (
                                    <tr key={val.id}>
                                        <td>{val.title}</td>
                                        <td>{val.artist}</td>
                                        <td>{val.album}</td>
                                        <td>{val.duration.toTime()}</td>
                                    </tr>
                                ))
                            }
                        </tbody>
                    </table>
                </div>
                <BottomNav />
            </div>
        );
    }
}

export default RootView;