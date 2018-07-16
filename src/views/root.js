import React from 'react';
import _ from 'underscore';
require('velocity-animate');
require('velocity-animate/velocity.ui');
import NavBar from './Navbar';
import BottomNav from './BottomNav';
import {musicTable, albumGrid} from './Content';
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
        var content;
        switch(this.props.music.route[0]){
            case 'songs':
                content = musicTable(this.props);
                break;
            case 'allAlbums':
                content = albumGrid(this.props);
                break;
            default:
                content = <p>You shouldn't see this!!!</p>;
                break;
        }

        return (
            <div>
                <NavBar {...this.props} addMusic={this.openFolders} />
                {content}
                <BottomNav />
            </div>
        );
    }
}

export default RootView;