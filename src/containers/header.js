import {connect} from 'react-redux';
import Navbar from '../components/navbar';
import {navigateUI, additionReady} from '../actions';

const mapStateToProps = state => {
    return {
        route: state.route,
        genres: state.genres
    }
};

const mapDispatchToProps = dispatch => {
    return {
        onNavClick: (tar, val) => {
            dispatch(navigateUI(tar, val));
        },
        importFolders: (data) => {
            dispatch(additionReady(data));
        }
    }
};

const Header = connect(mapStateToProps, mapDispatchToProps)(Navbar);

export default Header;