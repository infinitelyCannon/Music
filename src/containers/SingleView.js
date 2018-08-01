import {connect} from 'react-redux';
import DetailView from '../components/detailView';
import {navigateUI} from '../actions';

const mapStateToProps = state => {
    return {
        route: state.route,
        songs: state.songs,
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