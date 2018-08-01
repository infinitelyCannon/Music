import {connect} from 'react-redux';
import DetailView from '../components/detailView';
import _ from 'lodash';

const mapStateToProps = state => {
    return {
        route: state.route,
        songs: _.filter(state.songs, (song) => {return (song.album === state.route.view.album && song.albumArtist === state.route.view.by)}),
        albums: state.albums
    };
}

const mapDispatchToProps = dispatch => {
    return {
        onPlayStart: (target) => console.log("Playing " + target)
    }
};

const SingleView = connect(mapStateToProps, mapDispatchToProps)(DetailView);

export default SingleView;