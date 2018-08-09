import React from 'react';
import _ from 'lodash';
import Player from '../Player';
import {toTime} from '../util/Functions';

const Music = new Player();

class PlayBar extends React.Component{
    constructor(props){
        super(props);
        this.state = {
            seek: 0,
            shuffle: JSON.parse(window.localStorage.getItem("shuffle")) || false,
            repeat: window.localStorage.getItem("repeat") || 'repeat-once'
        };
        this.onPlayStart = this.onPlayStart.bind(this);
        this.onEndTrigger = this.onEndTrigger.bind(this);
        Music.setPlayTrigger(this.onPlayStart);
        Music.setDispatcher('end', this.onEndTrigger);
    }

    componentDidMount(){
        Music.setDispatcher('indx', this.props.onNewTrack);
        Music.setDispatcher('id', this.props.onNewSong);
    }

    componentDidUpdate(prevProps){
        if(_.isEqual(prevProps.player.queue, this.props.player.queue) && (this.props.player.index != Music.getIndex())){
            Music.skipTo(this.props.player.index);
        }
        if(this.props.player.queue[0] === 'new' && (this.props.player.index != Music.getIndex() && !_.isEqual(this.props.player.queue, prevProps.player.queue))){
            Music.beginPlayback(_.tail(this.props.player.queue), this.props.player.index, this.state.shuffle);
        }
    }

    step(){
        var seek = Music.getProgress();
        this.setState({seek: (typeof seek === 'number' ? seek : 0)});
        
        if(Music.isPlaying()){
            requestAnimationFrame(this.step.bind(this));    
        }
    }

    onPlayStart(){
        this.setState({seek: 0});

        requestAnimationFrame(this.step.bind(this));
    }

    onEndTrigger(){
        if(this.state.repeat === "repeat-once"){
            Music.play();
            return;
        }
        else if(this.state.repeat === "repeat-off"){
            if(this.props.player.index === this.props.player.queue.length - 2){
                Music.stop();
                this.props.onNewSong("");
                return;
            }
        }

        Music.skip('next');
    }

    handleNavClick(e){
        var playing = Music.getNowPlaying();

        if(e.target.innerHTML != ""){
            if(e.target.id === ""){
                this.props.onNavClick("view", {type: "album", album: playing.album, by: playing.albumArtist});
            }
            else{
                this.props.onNavClick("view", {type: "artist", artist: playing.albumArtist});
            }
        }
    }

    toggleRepeat(){
        switch(this.state.repeat){
            case "repeat-off":
                this.setState({repeat: "repeat"});
                window.localStorage.setItem("repeat", "repeat");
                break;
            case "repeat":
                this.setState({repeat: "repeat-once"});
                window.localStorage.setItem("repeat", "repeat-once");
                break;
            case "repeat-once":
            default:
                this.setState({repeat: "repeat-off"});
                window.localStorage.setItem("repeat", "repeat-off");
                break;
        }
    }

    toggleShuffle(){
        if(this.state.shuffle){
            Music.scramble();
        }
    }

    render(){
        var nowPlaying = undefined;
        if(this.props.player.nowPlaying != ''){
            nowPlaying = _.find(this.props.songs, (item) => {return item.id === this.props.player.nowPlaying});
        }

        return (
            <nav id="playbar" className="level is-fixed-bottom">
                <div id="playbar-left" className="level-left">
                    <div className="level-item">
                        <div className="media">
                            <figure className="media-left">
                                <p className="image is-96x96">
                                    <img src={nowPlaying != undefined ? nowPlaying.cover : ""} />
                                </p>
                            </figure>
                            <div className="media-content">
                                <a title={nowPlaying != undefined ? nowPlaying.title : ""} onClick={(e) => this.handleNavClick(e)} className="title is-5">{nowPlaying != undefined ? nowPlaying.title : ""}</a><br />
                                <a title={nowPlaying != undefined ? nowPlaying.artist : ""} onClick={(e) => this.handleNavClick(e)} id="nowArtist" className="subtitle">{nowPlaying != undefined ? nowPlaying.artist : ""}</a>
                            </div>
                        </div>
                    </div>
                </div>
                <div className="level-item has-text-centered" id="playbar-mid">
                    <div id="playing-btns" className="buttons">
                        <span id="time-start">{nowPlaying != undefined ? toTime(this.state.seek) : " "}</span>
                        <a className="button" onClick={() => this.toggleRepeat()}>
                            <span className="icon is-small">
                                <i className={"mdi mdi-" + this.state.repeat + (this.state.repeat === "repeat-off" ? "" : " is-highlighted")}></i>
                            </span>
                        </a>
                        <a className="button" onClick={() => {if(Music.isActive) Music.skip('prev')}}>
                            <span className="icon is-small">
                                <i className="mdi mdi-skip-previous"></i>
                            </span>
                        </a>
                        <a className="button" onClick={() => Music.playPause()}>
                            <span className="icon is-medium">
                                <i className={"mdi mdi-36px mdi-" + (Music.isPlaying() ? "pause" : "play")}></i>
                            </span>
                        </a>
                        <a className="button" onClick={() => {if(Music.isActive) Music.skip('next')}}>
                            <span className="icon is-small">
                                <i className="mdi mdi-skip-next"></i>
                            </span>
                        </a>
                        <a className="button" onClick={() => this.setState({shuffle: !this.state.shuffle})}>
                            <span className="icon is-small">
                                <i className={"mdi mdi-shuffle " + (this.state.shuffle ? "is-highlighted" : "")}></i>
                            </span>
                        </a>
                        <span id="time-end">{nowPlaying != undefined ? toTime(nowPlaying.duration) : " "}</span>
                        <div id="audio-track">
                            <input type="range" value={nowPlaying != undefined && Music.isActive ? ((this.state.seek / nowPlaying.duration) * 100) : 0} min="0" max="100" />
                            <progress value={nowPlaying != undefined && Music.isActive ? ((this.state.seek / nowPlaying.duration) * 100) : 0} className="progress is-small is-primary" max="100"></progress>
                        </div>
                    </div>
                </div>
                <div id="playbar-right" className="level-right">
                    <div className="level-item">
                        <p className="buttons">
                            <a className="button">
                                <span className="icon is-medium">
                                    <i className="mdi mdi-volume-off"></i>
                                </span>
                            </a>
                            <a className="button">
                                <span className="icon is-medium">
                                    <i className="mdi mdi-playlist-play"></i>
                                </span>
                            </a>
                        </p>
                    </div>
                </div>
            </nav>
        );
    }
}

export default PlayBar;