import Immutable from 'immutable';

const Playlist = Immutable.Record({
    name: '',
    songs: []
});

export default Playlist;