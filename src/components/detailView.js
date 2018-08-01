import React from 'react';
import _ from 'lodash';
import Img from 'react-image';

function albumDetail({route, songs, albums}){
    var target = _.find(albums, {title: route.view.album, artist: route.view.by});
    var music = _.sortBy(songs, (song) => {return song.trackNum.no});

    return (
        <div id="container">
            <article id="album-info" className="media">
                <figure className="image" style={{marginRight: '15px'}}>
                    <img style={{maxWidth: '200px', height: '200px', width: '200px'}} src={target.cover} />
                </figure>
                <div className="media-content">
                    <h3 className="title is-4">{target.title}</h3>
                    <h3 className="subtitle" style={{marginBottom: '0px'}}>{target.artist}</h3>
                    <h3 className="subtitle" style={{marginBottom: '5px'}}>{music.length} {music.length === 1 ? "song" : "songs"} &#8226; {target.year} &#8226; {target.genre}</h3>
                    <h3 className="subtitle buttons">
                        <a className="button">
                            <span className="icon is-medium">
                                <i className="mdi mdi-24px mdi-shuffle"></i>
                            </span>
                            <span>Shuffle</span>
                        </a>
                        <a className="button">
                            <span className="icon is-medium">
                                <i className="mdi mdi-24px mdi-play-circle-outline"></i>
                            </span>
                            <span>Play All</span>
                        </a>
                        <a className="button">
                            <span className="icon is-medium">
                                <i className="mdi mdi-24px mdi-dots-vertical"></i>
                            </span>
                        </a>
                    </h3>
                </div>
            </article>
            <table className="table is-fullwidth is-striped is-hoverable">
                <thead>
                    <tr>
                        <th>#</th>
                        <th>Title</th>
                        <th><span className="icon is-medium"><i className="mdi mdi-clock"></i></span></th>
                        <th>Artist</th>
                        <th><span title="Play Count" className="icon is-medium"><i className="mdi mdi-music-note"></i></span></th>
                    </tr>
                </thead>
                <tbody>
                    {
                        _.map(music, (item) => (
                            <tr key={item.id}>
                                <td>{(typeof item.trackNum.no === "number" ? item.trackNum.no : "-")}</td>
                                <td>{item.title}</td>
                                <td>{item.duration.toTime()}</td>
                                <td>{item.artist}</td>
                                <td>{item.playcount}</td>
                            </tr>
                        ))
                    }
                </tbody>
            </table>
        </div>
    );
}

function  artistDetail({route, artists, albums, onNameClick}){
    var target = _.find(artists, {name: route.view.artist});
    var music = _.sortBy(_.filter(albums, (item) => {return (item.artist == route.view.artist)}), 'title');

    return (
    <div id="container">
        <article id="album-info" className="media">
            <figure className="image" style={{marginRight: '15px'}}>
                <Img style={{maxWidth: '200px', height: '200px', width: '200px', borderRadius: '50%'}} src={[target.photo, 'dist/person.png']} />
            </figure>
            <div className="media-content">
                <h3 className="title is-4">{target.name}</h3>
                <h3 className="subtitle" style={{marginBottom: '5px'}}>{music.length} {music.length === 1 ? "album" : "albums"}</h3>
                <h3 className="subtitle">
                    <a className="button">
                        <span className="icon is-medium">
                            <i className="mdi mdi-24px mdi-play-circle-outline"></i>
                        </span>
                        <span>Play All</span>
                    </a>
                    <a className="button">
                        <span className="icon is-medium">
                            <i className="mdi mdi-24px mdi-shuffle"></i>
                        </span>
                        <span>Shuffle</span>
                    </a>
                </h3>
            </div>
        </article>
        <article className="message" style={{margin: '0'}}>
            <div className="message-header">
                <h3 className="title is-4">In Your Library</h3>
            </div>
        </article>
        <div className="row">
            {
                _.map(music, (item) => (
                    <div className="card" key={item.title}>
                        <div className="card-image" onClick={() => onNameClick("view", {type: "album", album: item.title, by: target.name})}>
                            <figure className="image is-1by1">
                                <img src={item.cover} />
                            </figure>
                        </div>
                        <div className="card-content">
                            <a onClick={() => onNameClick("view", {type: "album", album: item.title, by: target.name})} className="title is-4">{item.title}</a>
                            <br />
                            <span className="subtitle">{item.year}</span>
                        </div>
                    </div>
                ))
            }
        </div>
    </div>
    );
}

const DetailView = ({route, songs, albums, artists, onPlayStart, onNameClick}) => {
    switch(route.view.type){
        case "album":
            return albumDetail({route, songs, albums});
        case "artist":
            return artistDetail({route, artists, albums, onNameClick});
        default:
            return <p>Error: Invalid route detected.</p>
    }
}

export default DetailView;