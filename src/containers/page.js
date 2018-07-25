import {connect} from 'react-redux';
import Content from '../components/content';
//import {} from '../actions';

const mapStateToProps = state => {
    return {
        music: state[state.route.view],
        route: state.route
    }
};

const mapDispatchToProps = dispatch => {
    return {
        onNameClick: (target, value) => console.log("Opening " + value)
    }
};

const MainPage = connect(mapStateToProps, mapDispatchToProps)(Content);

export default MainPage;