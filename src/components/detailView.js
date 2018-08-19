import React from 'react';
import _ from 'lodash';
import Img from 'react-image';

function AlbumDetail(props){
    var target = _.find(props.albums, {title: props.route.view.album, artist: props.route.view.by});
    var music = _.sortBy(_.filter(props.songs, (item) => {
        return (item.album === props.route.view.album && item.albumArtist === props.route.view.by)
    }), (song) => {return song.trackNum.no});

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
                        _.map(music, (item, index) => (
                            <tr key={item.id}>
                                <td>{(typeof item.trackNum.no != null ? item.trackNum.no : "-")}</td>
                                <td><span onClick={() => props.onPlayStart(["new", ...music], index)} className="song-title">{item.title}</span></td>
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

function  ArtistDetail(props){
    var target = _.find(props.artists, {name: props.route.view.artist});
    var music = _.sortBy(_.filter(props.albums, (item) => {return (item.artist == props.route.view.artist)}), 'title');

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
                        <div className="card-image" onClick={() => props.onNameClick("view", {type: "album", album: item.title, by: target.name})}>
                            <figure className="image is-1by1">
                                <img src={item.cover} />
                            </figure>
                        </div>
                        <div className="card-content">
                            <div className="media">
                                <div className="media-content">
                                    <a title={item.title} onClick={() => props.onNameClick("view", {type: "album", album: item.title, by: target.name})} className="title is-4">{item.title}</a>
                                    <br />
                                    <span className="subtitle">{item.year}</span>
                                </div>
                            </div>
                        </div>
                    </div>
                ))
            }
        </div>
    </div>
    );
}

function SearchResults(props){
    var songResults = _.filter(props.songs, (song) => {
        return song.title.search(new RegExp(props.route.view.value, 'i')) != -1;
    });
    var albumResults = _.filter(props.albums, (album) => {
        return album.title.search(new RegExp(props.route.view.value, 'i')) != -1;
    });
    var artistResults = _.filter(props.artists, (artist) => {
        return artist.name.search(new RegExp(props.route.view.value, 'i')) != -1;
    });
    var messageCount = 1;

    return (
        <div id="container">
            <section className="hero">
                <div className="hero-body">
                    <span className="subtitle">
                        <h3 className="title is-3">Results For:</h3> {props.route.view.value}
                    </span>
                </div>
            </section>
            {
                artistResults.length > 0 && (
                    <article className="message" style={{zIndex: messageCount++, marginTop: '15px', marginBottom: '0'}}>
                        <div className="message-header">
                            <h3 className="subtitle">Artists</h3>
                        </div>
                    </article>
                )
            }
            {
                artistResults.length > 0 && (
                    <div className="row">
                        {
                            _.map(artistResults, (artist) => (
                                <div className="card artist" key={artist.name}>
                                    <div className="card-image" onClick={() => props.onNameClick("view", {type: "artist", artist: artist.name})}>
                                        <figure className="image is-1by1">
                                            <Img src={[artist.photo, 'dist/person.png']} />
                                        </figure>
                                    </div>
                                    <div className="card-content">
                                        <a onClick={() => props.onNameClick("view", {type: "artist", artist: artist.name})}></a>
                                    </div>
                                </div>
                            ))
                        }
                    </div>
                )
            }
            {
                albumResults.length > 0 && (
                    <article className="message" style={{zIndex: messageCount++, marginTop: '15px', marginBottom: '0'}}>
                        <div className="message-header">
                            <h3 className="subtitle">Albums</h3>
                        </div>
                    </article>
                )
            }
            {
                albumResults.length > 0 && (
                    <div className="row">
                        {
                            _.map(albumResults, (card) => (
                                <div key={card.title + '_' + card.artist} className="card">
                                    <div className="card-image" onClick={() => props.onNameClick("view", {album: card.title, by: card.artist, type: "album"})}>
                                        <figure className="image is-1by1">
                                            <img src={card.cover} />
                                        </figure>
                                    </div>
                                    <div className="card-content">
                                        <div className="media">
                                            <div className="media-content">
                                                <a onClick={() => props.onNameClick("view", {album: card.title, by: card.artist, type: "album"})} title={card.title} className="title is-5">{card.title}</a>
                                                <br />
                                                <a onClick={() => props.onNameClick("view", {type: "artist", artist: card.artist})} title={card.artist} className="subtitle is-6">{card.artist}</a>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            ))
                        }
                    </div>
                )
            }
            {
                songResults.length > 0 && (
                    <article className="message" style={{zIndex: messageCount++, marginTop: '15px', marginBottom: '0'}}>
                        <div className="message-header">
                            <h3 className="subtitle">Songs</h3>
                        </div>
                    </article>
                )
            }
            {
                songResults.length > 0 && 
                <table className="table is-fullwidth is-striped is-hoverable">
                    <thead>
                        <tr>
                            <th>Name</th>
                            <th>
                                <span className="icon is-medium">
                                    <i className="mdi mdi-24px mdi-clock-outline"></i>
                                </span>
                            </th>
                            <th>Artist</th>
                            <th>Album</th>
                        </tr>
                    </thead>
                    <tbody>
                        {
                            _.map(songResults, (item) => (
                                <tr key={item.id}>
                                    <td>{item.title}</td>
                                    <td>{item.duration.toTime()}</td>
                                    <td>{item.artist}</td>
                                    <td>{item.album}</td>
                                </tr>
                            ))
                        }
                    </tbody>
                </table>
            }
        </div>
    );
}

class DetailView extends React.Component{
    constructor(props){
        super(props);
        this.state = {
            selected: []
        };
    }

    render(){
        switch(this.props.route.view.type){
            case "album":
                return <AlbumDetail {...this.props} />
            case "artist":
                return <ArtistDetail {...this.props} />
            case "search":
                return <SearchResults {...this.props} />
            default:
                return <p>Error: Invalid route detected, {this.props.route.view.type}</p>
        }
    }
}

/* const DetailView = ({route, songs, albums, artists, onPlayStart, onNameClick}) => {
    switch(route.view.type){
        case "album":
            return albumDetail({route, songs, albums, onPlayStart});
        case "artist":
            return artistDetail({route, artists, albums, onNameClick});
        case "search":
            return searchResults({route, songs, albums, artists, onNameClick});
        default:
            return <p>Error: Invalid route detected, {route.view.type}</p>
    }
} */

export default DetailView;