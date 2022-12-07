import { useEffect, useState } from 'react';
import styled from 'styled-components';
import { espPrint, espSetState } from './App';
import './fonts.css';


const Name = styled.span`
  color: black;
  position: relative;
    font-size: 100px;
    top: 50px;
    width: 1000px;
    display: flex;
  justify-content: center;
  text-align: center;

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
  color: black;
  position: absolute;
  width: 1000px;
  top: 70px;
  left: 240px;
  font-size: 100px;
  display: flex;
  justify-content: center;

    display: inline-block;
    max-height: 100px;
    vertical-align: top;
    height: 100px;
    overflow: hidden;
    z-index: 100;

  & span {
    width: 700px;
    display: block;
    position: absolute;
    animation-duration: 4s;
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

export const firstNames = [
  'Julie',
  'Agnieszka',
  'Andrew',
  'Anton',
  'Dan',
  'Danny',
  'Gary',
  'Jamie',
  'Janet',
  'Lewis',
  'Marrick',
  'Matt',
  'Michael',
  'Mike',
  'Paul C',
  'Paul O',
  'Richard G',
  'Richard H',
  'Sinead',
  'Virgilus'
];

function getNamesToDisplay() {
  const newArray: string[] = [];
  while(newArray.length < 7) {
    const randomElement = firstNames[Math.floor(Math.random() * firstNames.length)];
    if (!newArray.includes(randomElement)) {
      newArray.push(randomElement)
    }
  }
  return newArray;
}
function Names(props: { name: string }) {
  const fullNames = [
    'Julie Purves',
    'Agnieszka Zuchowicz',
    'Andrew Donovan',
    'Anton Flugge',
    'Dan Hedges',
    'Danny Ronsayro',
    'Gary Lee',
    'Jamie McConnell',
    'Janet Wilson',
    'Lewis Jackson-McKetty',
    'Marrick Somal',
    'Matt Parsons',
    'Michael Rowe',
    'Mike Darnell',
    'Paul Carrington',
    'Paul Owen',
    'Richard Grills',
    'Richard Harding',
    'Sinead Charles',
    'Virgilus Chebe'
  ];
    const [nameReady, setNameReady] = useState(false);
    useEffect(() => {
      console.log("sending name");  
      fetch(espPrint, {
            method: 'POST',
            body: props.name === 'Julie' ? `naughty`: fullNames[firstNames.indexOf(props.name)],
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
        });}, 4000)
    }, [])
    // const namesToDisplay = getNamesToDisplay();
    return (
        <>
            {!nameReady && <StyledNames>
                {getNamesToDisplay().map((name: string) => <span className="title-slide">{name}</span>)}
            </StyledNames>}
            {props.name && nameReady && <Name>{props.name}</Name>}
        </>
    );
}

export default Names;
