import React from 'react';
import _ from 'lodash';

const Albums = ({music, onNameClick, route}) => {
    return (
        _.sortBy(music, (item) => {return item[route.view]}).map((card) => (
            <div key={card.title + '_' + card.artist} className="card">
                <div className="card-image">
                    <figure className="image is-1by1">
                        <img src={card.cover} />
                    </figure>
                </div>
                <div className="card-content">
                    <div className="media">
                        <div className="media-content">
                            <a onClick={() => onNameClick("view", "#" + card.title + '_' + card.artist)} title={card.title} className="title is-5">{card.title}</a>
                            <br />
                            <a title={card.artist} className="subtitle is-6">{card.artist}</a>
                        </div>
                    </div>
                </div>
            </div>
        ))
    );
};

export default Albums;