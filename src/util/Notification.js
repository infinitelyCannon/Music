import React from 'react';
//import VelocityComponent from 'velocity-react/velocity-component';
import VelocityTransitionGroup from 'velocity-react/velocity-transition-group';
import _ from 'lodash';

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

function ToastSlice(props){
    switch(props.info.type){
        case "msg":
            setTimeout(() => window.removeToast(props.info.id), 4500);
            return (
                <div className="box toast" style={{top: (93 + (70 * (props.size - (props.index + 1) ))) + "px"}}>
                    <h1>{props.info.msg}</h1>
                    <a className="button" onClick={() => window.removeToast(props.info.id)}>
                        <span className="icon is-small">
                            <i className="mdi mdi-18px mdi-close"></i>
                        </span>
                    </a>
                </div>
            );
        default:
            return <span></span>;
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

    appendToast(data, id = Date.now()){
        let items = this.state.drawer;

        items.push(Object.assign({}, data, {id: id}));

        this.setState({drawer: items});
    }

    updateToast(id){
        let items = this.state.drawer;
        let freshToast = _.find(items, (n) => {return n.id === id});

        items = _.union([freshToast], items);

        this.setState({drawer: items});
    }

    removeToast(id){
        let items = this.state.drawer;

        items = _.filter(items, (item) => {return item.id != id});

        this.setState({drawer: items});
    }

    render(){
        return (
        <VelocityTransitionGroup enter={EntryAnim} leave={ExitAnim}>
            {
                this.state.drawer.map((item, idx) => <ToastSlice key={item.id} info={item} index={idx} size={this.state.drawer.length} />)
            }
        </VelocityTransitionGroup>
        );
    }
}

export default Notifications;