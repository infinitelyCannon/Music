import {connect} from 'react-redux';
import DetailView from '../components/detailView';
import {navigateUI} from '../actions';
import _ from 'lodash';

const mapStateToProps = state => {
    return {
        route: state.route,
        songs: _.filter(state.songs, (song) => {return (song.album === state.route.view.album && song.albumArtist === state.route.view.by)}),
        albums: state.albums,
        artists: state.artists
    };
}

const mapDispatchToProps = dispatch => {
    return {
        onPlayStart: (target) => console.log("Playing " + target),
        onNameClick: (target, value) => dispatch(navigateUI(target, value))
    }
};

const SingleView = connect(mapStateToProps, mapDispatchToProps)(DetailView);

export default SingleView;