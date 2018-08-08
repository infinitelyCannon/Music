import {connect} from 'react-redux';
import PlayBar from '../components/playbar';

const mapStateToProps = state => {
    return {
        player: state.player,
        songs: state.songs
    }
};

const Footer = connect(mapStateToProps)(PlayBar);

export default Footer;