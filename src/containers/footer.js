import {connect} from 'react-redux';
import PlayBar from '../components/playbar';
import {navigateUI, updateIndex, updateNowPlaying} from '../actions';

const mapStateToProps = state => {
    return {
        player: state.player,
        songs: state.songs
    }
};

const mapDispatchToProps = dispatch => {
    return {
        onNavClick: (tar, val) => {
            dispatch(navigateUI(tar, val));
        },
        onNewTrack: (num) => {
            dispatch(updateIndex(num));
        },
        onNewSong: (str) => {
            dispatch(updateNowPlaying(str));
        }
    };
}

const Footer = connect(mapStateToProps, mapDispatchToProps)(PlayBar);

export default Footer;