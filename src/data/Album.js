import Immutable from 'immutable';

const Album = Immutable.Record({
    name: '',
    year: 0,
    size: 0,
    artist: '',
    genre: '',
    cover: [],
    coverType: '',
    range: {start: 0, end:0}
});

export default Album;