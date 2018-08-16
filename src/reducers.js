import {combineReducers} from 'redux';
import {ADD_SONGS, NAVIGATE_UI, SCAN_DIRECTORIES, ADDITION_FAILURE, ADDITION_READY, NAVIGATE_HISTORY, ADD_TO_QUEUE, UPDATE_INDEX, UPDATE_NOW_PLAYING} from './actions';
import _ from 'lodash';
import hash from 'hash-sum';
const initialState = {
    route: {
        view: "albums",
        sort: {type: "title", ascend: true},
        filter: "All",
        history: {index: 0, list: ["albums"]}
    },
    songs: window.eStore.get('songs') || [],
    albums: window.eStore.get('albums') || [],
    genres: window.eStore.get('genres') || [],
    artists: window.eStore.get('artists') || [],
    player: {
        queue: [],
        index: -1,
        nowPlaying: ''
    }
};

function safeImg(data){
    return (data != undefined ? window.dataPath + '/img/' + data.id + '.' + data.type : 'dist/blankCover.png');
}

function safeVal(value, fallback){
    if(value === undefined || value === null){
        return fallback;
    }
    if(typeof value === 'object' && value.length != undefined){
        return _.uniq(value).toString().replace(',', ', ');
    }
    if(typeof value === 'string'){
        return value.trim();
    }
    else{
        return value;
    }
}

function player(state = initialState.player, action){
    switch(action.type){
        case ADD_TO_QUEUE:
            return Object.assign({}, {queue: action.list, index: action.start});
        case UPDATE_INDEX:
            return Object.assign({}, state, {index: action.index});
        case UPDATE_NOW_PLAYING:
            return Object.assign({}, state, {nowPlaying: action.id});
        default:
            return state;
    }
}

function route(state = initialState.route, action){
    switch(action.type){
        case NAVIGATE_UI:
            var hist;
            if(state.history.list.length > state.history.index + 1){
                hist = _.slice(state.history.list, 0, state.history.index + 1);
                hist.push(action.value);
            }
            else{
                hist = [...state.history.list, action.value];
            }
            return Object.assign({}, state, {[action.target]: action.value, history: {list: hist, index: hist.length - 1}});
        case NAVIGATE_HISTORY:
            var idx = state.history.index;
            if(action.goForward){
                if((idx + 1) < state.history.list.length){
                    return Object.assign({}, state, {view: state.history.list[idx + 1], history: {list: state.history.list, index: idx + 1}});
                }
                else{
                    return state;
                }
            }
            else{
                if((idx - 1) >= 0){
                    return Object.assign({}, state, {view: state.history.list[idx - 1], history: {list: state.history.list, index: idx - 1}});
                }
                else{
                    return state;
                }
            }
        default:
            return state;
    }
}

function genres(state = initialState.genres, action){
    let toMerge;

    switch(action.type){
        case ADDITION_READY:
            toMerge = _.map(action.data, (item) => {return safeVal(item.common.genre, "All")});
            return _.union(state, toMerge);
        default:
            return state;
    }
}

function songs(state = initialState.songs, action){
    let toMerge = [];

    switch(action.type){
        case ADD_SONGS:
            return state;
        case SCAN_DIRECTORIES:
            return "pending";
        case ADDITION_READY:
            for(let i = 0; i < action.data.length; i++){
                if(action.data[i].format.duration === undefined){
                    console.warn(action.data[i]);
                }
                toMerge.push({
                    id: action.data[i].fileName,
                    title: safeVal(action.data[i].common.title, action.data[i].name),
                    artist: safeVal(action.data[i].common.artists, "Unknown Artist"),
                    albumArtist: safeVal(action.data[i].common.albumartist, safeVal(action.data[i].common.artists, "Unknown Artist")),
                    album: safeVal(action.data[i].common.album, "Unknown Album"),
                    year: safeVal(action.data[i].common.year, NaN),
                    genre: safeVal(action.data[i].common.genre, ""),
                    duration: action.data[i].format.duration,
                    cover: safeImg(action.data[i].common.picture),
                    trackNum: safeVal(action.data[i].common.track, NaN),
                    diskNum: safeVal(action.data[i].common.disk, NaN),
                    playcount: 0,
                    isMultiSong: false,
                    isFragment: false,
                    fragments: []
                });
            }
            return _.unionBy(state, toMerge, "id");
        default:
            return state;
    }
}

function albums(state = initialState.albums, action){
    switch(action.type){
        case ADDITION_READY:
            let albums = [];
            //let oldState = state;
            for(let i = 0; i < action.data.length; i++){
                let tempIndex = _.find(albums, (item) => {return item.title === safeVal(action.data[i].common.album, "Unknown Album")});
                if(tempIndex === undefined){
                    albums.push(Object.assign({}, {
                        title: safeVal(action.data[i].common.album, "Unknown Album"),
                        year: safeVal(action.data[i].common.year, NaN),
                        artist: safeVal(action.data[i].common.albumartist, safeVal(action.data[i].common.artists, "Unknown Artist")),
                        genre: safeVal(action.data[i].common.genre, ""),
                        cover: safeImg(action.data[i].common.picture)
                    }));
                }
                else{
                    if(action.data[i].common.track.no === 1){
                        albums = _.map(albums, (item) => {
                            if(item.title === safeVal(action.data[i].common.album, "Unknown Album")){
                                return {
                                    title: safeVal(action.data[i].common.album, "Unknown Album"),
                                    year: safeVal(action.data[i].common.year, NaN),
                                    artist: safeVal(action.data[i].common.albumartist, safeVal(action.data[i].common.artists, "Unknown Artist")),
                                    genre: safeVal(action.data[i].common.genre, ""),
                                    cover: safeImg(action.data[i].common.picture)
                                }
                            }
                            else{
                                return item;
                            }
                        });
                    }
                }
            }
            return _.unionBy(albums, state, "title");
        default:
            return state;
    }
}

function artists(state = initialState.artists, action){
    switch(action.type){
        case ADDITION_READY:
            let result = _.map(action.data, (item) => {
                return {
                    name: safeVal(item.common.albumartist, safeVal(item.common.artists, "Unknown Artist")),
                    photo: window.dataPath + '/img/' + hash(safeVal(item.common.albumartist, safeVal(item.common.artists, "Unknown Artist"))) + '.jpeg'
                }
            });
            return _.unionBy(state, result, "name");
        default:
            return state;
    }
}

const rootReducer = combineReducers({
    route: route,
    songs: songs,
    albums: albums,
    genres: genres,
    artists: artists,
    player: player
});

export default rootReducer;