import React from 'react';

class PlayBar extends React.Component{
    constructor(props){
        super(props);
    }

    render(){
        return (
            <nav className="level is-fixed-bottom">
                <div className="level-left">
                    <div className="level-item">
                        <div className="media">
                            <figure className="media-left">
                                <p className="image is-96x96">
                                    <img src="dist/blankCover.png" />
                                </p>
                            </figure>
                            <div className="media-content">
                                <h5 className="title is-5">Song Title</h5>
                                <p>Artist</p>
                            </div>
                        </div>
                    </div>
                </div>
                <div className="level-item has-text-centered">
                    <div id="playing-btns" className="buttons">
                        <span id="time-start">0:00</span>
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
                                <i className="mdi mdi-skip-next"></i>
                            </span>
                        </a>
                        <a className="button">
                            <span className="icon is-small">
                                <i className="mdi mdi-shuffle"></i>
                            </span>
                        </a>
                        <span id="time-end">5:23</span>
                        <div id="audio-track">
                            <input type="range" min="0" max="100" />
                            <progress className="progress is-small is-primary" value="50" max="100"></progress>
                        </div>
                    </div>
                </div>
                <div className="level-right">
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