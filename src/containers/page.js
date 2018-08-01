import {connect} from 'react-redux';
import Content from '../components/content';
import {navigateUI} from '../actions';

const mapStateToProps = state => {
    return {
        music: state[state.route.view],
        route: state.route
    }
};

const mapDispatchToProps = dispatch => {
    return {
        onNameClick: (target, value) => dispatch(navigateUI(target, value))
    }
};

const MainPage = connect(mapStateToProps, mapDispatchToProps)(Content);

export default MainPage;