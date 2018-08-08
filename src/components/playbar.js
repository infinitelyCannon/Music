import React from 'react';
import _ from 'lodash';
import Player from '../Player';

const Music = new Player();

class PlayBar extends React.Component{
    constructor(props){
        super(props);
    }

    componentDidUpdate(prevProps){
        if(this.props.player.queue[0] === 'new'){
            Music.beginPlayback(_.tail(this.props.player.queue), this.props.player.index);
        }
    }

    render(){
        return <div />
    }
    /*
    constructor(props){
        super(props);
        this.state = {
            queue: [],
            progress: 0,
            duration: 0,
            index: 0,
            isActive: false
        };
    }

    beginPlayback(list, start){
        if(this.state.isActive){
            this.state.queue[this.state.index].howl.stop();
        }
        var result = _.map(list, (item) => {
            return Object.assign({}, {id: item, howl: null});
        });

        this.setState({
            queue: result,
            index: start
        }, () => this.play(start));
    }

    play(index){
        var sound;
        console.log(this.state.queue.length);
        index = typeof index === 'number' ? index : this.state.index;
        var track = this.state.queue[index];

        if(track.howl){
            sound = track.howl;
        }
        else{
            sound = this.state.queue[index].howl = new Howl({
                src: [track.id],
                html5: true,
                loop: true,
                onplay: this.playStart
            });
        }

        sound.play();
        this.setState({isActive: true, index: index});
    }

    componentDidUpdate(prevProps){
        if(this.props.player.queue[0] === "new" && !_.isEqual(prevProps.player.queue, this.props.player.queue)){
            this.beginPlayback(_.tail(this.props.player.queue), this.props.player.index);
        }
    }

    playStart(){
        if(this.state.player === undefined){console.warn("Player not found")}
        var track = this.state.queue[this.state.index];
        this.setState({duration: track.duration()});
        requestAnimationFrame(this.step);
    }

    step(){
        var track = this.state.queue[this.state.index];
        var seek = track.seek() || 0;

        this.setState({progress: seek});
        if(track.howl.playing()){
            requestAnimationFrame(this.step);
        }
    }

    render(){
        var palying = (this.state.isActive ? _.find(this.props.songs, ['id', this.state.queue[this.state.index]]) : undefined);

        return (
            <nav className="level is-fixed-bottom">
                <div className="level-left" style={{flexBasis: '21%'}}>
                    <div className="level-item">
                        <div className="media">
                            <figure className="media-left">
                                <p className="image is-96x96">
                                    <img src="dist/blankCover.png" />
                                </p>
                            </figure>
                            <div className="media-content">
                                <h5 className="title is-5">{palying != undefined ? palying.title : ""}</h5>
                                <p>{palying != undefined ? palying.artist : ""}</p>
                            </div>
                        </div>
                    </div>
                </div>
                <div className="level-item has-text-centered" style={{flexBasis: '67%'}}>
                    <div id="playing-btns" className="buttons">
                        <span id="time-start">&nbsp;</span>
                        <a className="button">
                            <span className="icon is-small">
                                <i className="mdi mdi-repeat"></i>
                            </span>
                        </a>
                        <a className="button">
                            <span className="icon is-small">
                                <i className="mdi mdi-skip-previous"></i>
                            </span>
                        </a>
                        <a className="button">
                            <span className="icon is-medium">
                                <i className="mdi mdi-36px mdi-play"></i>
                            </span>
                        </a>
                        <a className="button">
                            <span className="icon is-small">
                                <i className="mdi mdi-skip-next"></i>
                            </span>
                        </a>
                        <a className="button">
                            <span className="icon is-small">
                                <i className="mdi mdi-shuffle"></i>
                            </span>
                        </a>
                        <span id="time-end">&nbsp;</span>
                        <div id="audio-track">
                            <input type="range" min="0" max="100" />
                            <progress className="progress is-small is-primary" value={this.state.isActive ? (this.state.progress / this.state.duration) : 0} max="100"></progress>
                        </div>
                    </div>
                </div>
                <div className="level-right" style={{flexBasis: '11%'}}>
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
    */
}

export default PlayBar;