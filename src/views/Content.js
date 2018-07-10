import React from 'react';

class Content extends React.Component{
    constructor(props){
        super(props);
        this.openFolders = this.openFolders.bind(this);
    }

    openFolders(){
        window.dialog.showOpenDialog({
            title: "Select a folder",
            properties: ["openDirectory"]
        }, (folderPaths) =>{
            if(folderPaths === undefined){
                console.log("No destination folder selected");
                return;
            }
            else{
                
            }
        });
    }

    render(){
        if(props.songs.isEmpty()){
            return <button onClick={this.openFolders}>Add Folder(s)</button>;
        }
        else{
            return <h1>Hello World!</h1>;
        }
    }
}

export default Content;