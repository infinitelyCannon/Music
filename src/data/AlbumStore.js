import Immutable from 'immutable';
import Album from './Album';
import {ReduceStore} from 'flux/utils';
import MusicDispatcher from './MusicDispatcher';
import ActionTypes from './ActionTypes';
import SongStore from './SongStore';
import SettingStore from './SettingStore';
import _ from 'underscore';

class AlbumStore extends ReduceStore{
    constructor(){
        super(MusicDispatcher);
    }

    getInitialState(){
        let albums = window.eStore.get('albums');
        let neededView = SettingStore.getState().route;

        if(neededView[0] === "allAlbums"){
            return (albums === undefined ? Immutable.List() : Immutable.List(albums));
        }
        else{
            return null;
        }
    }

    reduce(state, action){
        let database, first, item, collection, count, index, start;
        const list = Immutable.List();

        let data = window.eStore.get("albums");

        switch(action.type){
            case ActionTypes.ADD_SONG:
                break;
            default:
                return state;
        }

        if(action.type == ActionTypes.SYNC_ALBUM){
            database = SongStore.getState().toArray();
            collection = _.uniq(database, false, function(n){
                return n.album;
            });
            for(let i = 0; i < collection.length; i++){
                count = 0;
                item = _.findWhere(database, {album: collection[i]});
                if(item == undefined){
                    console.log(database);
                    console.error("Could not retrieve metadata.");
                }
                first = item;

                index = start = _.indexOf(database, item, false);
                while(database[index].album == item.album && index < database.length){
                    count++;
                    if(database[index].trackNum == 1){
                        first = database[index];
                    }
                    index++;
                }

                list.push(new Album({
                    name: first.album,
                    year: first.year,
                    size: count,
                    artist: first.albumArtist,
                    genre: first.genre,
                    cover: first.cover,
                    coverType: first.coverType,
                    range: {start: start, end: index - 1}
                }));
            }

            return list;
        }
        else{
            return state;
        }
    }
}

export default new AlbumStore();