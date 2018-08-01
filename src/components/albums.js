import React from 'react';
import _ from 'lodash';

function sorting(values, params){
    var stageOne = _.sortBy(values, (item) => {
        if(typeof item[params.sort.type] === 'string'){
            return item[params.sort.type].toLowerCase().replace(/^a\s+|^the\s+/gi, '');
        }
        else{
            return item[params.sort.type];
        }
    });

    if(params.filter != "All"){
        stageOne = _.filter(stageOne, {genre: params.filter});
    }

    return () => {
        if(!params.sort.ascend){
            return _.reverse(stageOne);
        }
        else{
            return stageOne;
        }
    }
}

const Albums = ({music, onNameClick, route}) => {
    return (
        _.map(sorting(music, route)(), (card) => (
            <div key={card.title + '_' + card.artist} className="card">
                <div className="card-image" onClick={() => onNameClick("view", {album: card.title, by: card.artist, type: "album"})}>
                    <figure className="image is-1by1">
                        <img src={card.cover} />
                    </figure>
                </div>
                <div className="card-content">
                    <div className="media">
                        <div className="media-content">
                            <a onClick={() => onNameClick("view", {album: card.title, by: card.artist, type: "album"})} title={card.title} className="title is-5">{card.title}</a>
                            <br />
                            <a onClick={() => onNameClick("view", {type: "artist", artist: card.artist})} title={card.artist} className="subtitle is-6">{card.artist}</a>
                        </div>
                    </div>
                </div>
            </div>
        ))
    );
};

export default Albums;