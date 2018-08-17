import React from 'react';
//import VelocityComponent from 'velocity-react/velocity-component';
import VelocityTransitionGroup from 'velocity-react/velocity-transition-group';

const EntryAnim = {
    animation: {
        opacity: 1,
        right: '0%'
    }
};

const ExitAnim = {
    animation: {
        opacity: 0,
        right: '-110%'
    }
}

class Notifications extends React.Component{
    constructor(props){
        super(props);
        this.state = {
            drawer: []
        };
        window.addToast = this.appendToast.bind(this);
        window.removeToast = this.removeToast.bind(this);
    }

    appendToast(type){
        let items = this.state.drawer;

        items.push({msg: type});

        this.setState({drawer: items});
    }

    removeToast(id){
        let items = this.state.drawer;

        items.pop();

        this.setState({drawer: items});
    }

    render(){
        return (
        <VelocityTransitionGroup enter={EntryAnim} leave={ExitAnim}>
            {
                this.state.drawer.map((item, idx) => (
                    <div key={item.msg} className="box toast" style={{top: (93 + (70 * (this.state.drawer.length - (idx + 1) ))) + "px"}}>
                        <h1>{item.msg}</h1>
                    </div>
                ))
            }
        </VelocityTransitionGroup>
        );
    }
}

export default Notifications;