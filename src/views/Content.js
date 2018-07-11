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
            return (
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
            );
        }
    }
}

export default Content;