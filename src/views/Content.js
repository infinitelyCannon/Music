import React from 'react';
import _ from 'underscore';

function musicTable(props){
    return (
        <div id="container" style={{paddingTop: '20px'}}>
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
                        [...props.music.songs.values()].map((val) => (
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
    );
}

function albumGrid(props){
    return (
        <div id="container" style={{paddingTop: '20px'}}>
            {
                _.sortBy([...props.music.albums.values()], 'name').map((val) => (
                    <div key={val.name + '_' + val.artist} className="card">
                        <div className="card-image">
                            <figure className="image is-1by1">
                                <img src={val.cover} />
                            </figure>
                        </div>
                        <div className="card-content">
                            <div className="media">
                                <div className="media-content">
                                    <span title={val.name} className="title is-5">{val.name}</span>
                                    <br />
                                    <span title={val.artist} className="subtitle is-6">{val.artist}</span>
                                </div>
                            </div>
                        </div>
                    </div>
                ))
            }
        </div>
    );
}

 /*
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
*/

export {musicTable, albumGrid};