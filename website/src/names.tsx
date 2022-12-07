import { useEffect, useState } from 'react';
import styled from 'styled-components';
import './fonts.css';


const espPrint  = "http://192.168.1.195/print";
const espSetState  = "http://192.168.1.195/setState";
const serverIP = "http://192.168.1.171:8081";



const Name = styled.span`
  color: goldenrod;
  position: relative;
    font-size: 100px;
    width: 100%;
    bottom: 580px;
    display: flex;
  justify-content: center;
  text-align: center;
  right: 420px;

  display: inline-block;
    max-height: 300px;
    position: relative;
    height: 300px;
    overflow: hidden;
    z-index: 100;

    width: 100%;
    display: block;
    position: absolute;
`

//https://codepen.io/PrimarchtheMage/pen/NWjMePo
const StyledNames = styled.div`
  color: goldenrod;
  position: relative;
  width: 100%;
  bottom: 300px;
  font-size: 100px;
  display: flex;
  justify-content: center;

    display: inline-block;
    max-height: 150px;
    vertical-align: top;
    position: relative;
    height: 300px;
    overflow: hidden;
    z-index: 100;

  & span {
    width: 100%;
    display: block;
    position: absolute;
    animation-duration: 1s;
    animation-timing-function: ease-in-out;
    animation-iteration-count: infinite;
    text-align: center;
  }

  & span:first-child {
    display: inline;
  }

@keyframes anim-1 {
    0% { bottom: 0;}
  15% { bottom: 110%;}
    84% { opacity: 0; bottom: 110%;}
    85% { bottom: -110%; opacity: 0;}
  86% {bottom: -110%; opacity: 1;}
    100% { bottom: 0;}
}

@keyframes anim-2 {
	0% { bottom: -110%;}
  15% { bottom: 0;}
  30% { bottom: 110%; opacity: 1;}
    31% { opacity: 0; bottom: 110%;}
    100% { bottom: -110%;opacity: 0}
}

@keyframes anim-3 {
    0%, 16%{ bottom: -110%;}
  30% { bottom: 0;}
  44% { bottom: 110%; opacity: 1;}
    45% { opacity: 0; bottom: 110%;}
    100% { bottom: -110%;opacity: 0}
}

@keyframes anim-4 {
	0%, 30% { bottom: -110%;}
  44% { bottom: 0;}
  58% { bottom: 110%; opacity: 1;}
    59% { opacity: 0; bottom: 110%;}
    100% { bottom: -110%;opacity: 0}
}

@keyframes anim-5 {
    0%, 44% { bottom: -110%;}
  58% { bottom: 0;}
  72% { bottom: 110%; opacity: 1;}
    73% { opacity: 0; bottom: 110%;}
    100% { bottom: -110%;opacity: 0}
}

@keyframes anim-6 {
	0%, 58% { bottom: -110%;}
  72% { bottom: 0;}
  86% { bottom: 110%; opacity: 1;}
    87% { opacity: 0; bottom: 110%;}
    100% { bottom: -110%;opacity: 0}
}

@keyframes anim-7 {
	0%, 72% { bottom: -110%;}
  86% { bottom: 0;}
  100% { bottom: 110%;}
}

  .title-slide:nth-child(1) {
    animation-name: anim-1;
  }
  .title-slide:nth-child(2) {
    animation-name: anim-2;
  }
  .title-slide:nth-child(3) {
    animation-name: anim-3;
  }
  .title-slide:nth-child(4) {
    animation-name: anim-4;
  }
  .title-slide:nth-child(5) {
    animation-name: anim-5;
  }
  .title-slide:nth-child(6) {
    animation-name: anim-6;
  }
  .title-slide:nth-child(7) {
    animation-name: anim-7;
  }
`;


function Names(props: { name: string }) {
    const [nameReady, setNameReady] = useState(false);
    useEffect(() => {
      console.log("sending name");  
      fetch(espPrint, {
            method: 'POST',
            body: props.name === 'Julie' ? `naughty`: `${props.name}`,
            headers: {
              'content-type': 'text/plain'
            }
          });
        setTimeout(() => { console.log("setState 3"); setNameReady(true); fetch(espSetState, {
          method: 'POST',
          body: '3',
          headers: {
            'content-type': 'text/plain'
          }
        });}, 3000)
    }, [])
    return (
        <>
            {!nameReady && <StyledNames>
                <span className="title-slide">Andy</span>
                <span className="title-slide">Anton</span>
                <span className="title-slide">Julie</span>
                <span className="title-slide">Michael</span>
                <span className="title-slide">Lewis</span>
                <span className="title-slide">Em</span>
            </StyledNames>}
            {props.name && nameReady && <Name>{props.name}</Name>}
        </>
    );
}

export default Names;
