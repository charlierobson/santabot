import React, { useEffect, useState } from 'react';
import logo from './logo.svg';
import face from './face.svg';
import eye from './eye.svg';
import body from './body.svg';
import nose from './nose.svg';
import mouth from './mouth--smile.svg';
import hal from './hal2.webp';
import beard from './beard.png';
import hair from './hair.png';
import styled from 'styled-components';
import './fonts.css';
import { io } from "socket.io-client";
import Names from './names';
import Sleeping from './sleeping';

/* 1920 x 1200 */
const PageContainer = styled.div`
  width: 1600px;
  height: 1000px;
  display: flex;
  justify-content: center;
  background-color: lightblue;
  @keyframes sliding {
  0% {
    background-position: 1024px 4096px, 0 3072px;
  }
  100% {
    background-position: 0 0, 1024px 0;
  }
}
  ${(props: { glitchy: boolean }) => {
    return props.glitchy ? `
background-image: url(https://helderman.github.io/blend-smoke/html5/noise1.jpg), url(https://helderman.github.io/blend-smoke/html5/noise2.jpg);
background-blend-mode: difference, normal;
    animation: sliding 5s linear infinite;
  ` : 'background-color: lightblue;';
  }}

  @keyframes colourChange {
    0% {
      background-color: lightblue;
    }
    100% {
      background-color: black;
    }
  }
  ${(props: { glitchy2: boolean }) => {
    return props.glitchy2 ? `
      animation: colourChange 0.5s infinite;
  ` : '';
  }}
`;

const Page = styled.div`
  position: relative;
  width: 1200px;
  height: 1000px;
  overflow: hidden;
  display: flex;
  align-items: center;
  justify-content: center;
  background-color: lightblue;
  font-size: 1px;
  @keyframes sliding {
  0% {
    background-position: 1024px 4096px, 0 3072px;
  }
  100% {
    background-position: 0 0, 1024px 0;
  }
}

@keyframes colourChange {
  0% {
    background-color: lightblue;
  }
  100% {
    background-color: black;
  }
}
  ${(props: { glitchy: boolean }) => {
    return props.glitchy ? `
background-image: url(https://helderman.github.io/blend-smoke/html5/noise1.jpg), url(https://helderman.github.io/blend-smoke/html5/noise2.jpg);
background-blend-mode: difference, normal;
width: 100%;
    animation: sliding 5s linear infinite;
  ` : 'background-color: lightblue;';
  }}

  ${(props: { glitchy2: boolean }) => {
    return props.glitchy2 ? `
      animation: colourChange 0.5s infinite;
  ` : '';
  }}
`

const SantaFace = styled.div`
  background: url(${face}) 0 0/contain no-repeat;
  bottom: 335px;
  height: 497em;
  position: absolute;
  width: 558em;
  font-size: 0.8px;

  @keyframes glitch1 {
  0% {
    transform: none;
    opacity: 1;
  }
  7% {
    transform: skew(-0.5deg, -0.9deg);
    opacity: 0.75;
  }
  10% {
    transform: none;
    opacity: 1;
  }
  27% {
    transform: none;
    opacity: 1;
  }
  30% {
    transform: skew(0.8deg, -0.1deg);
    opacity: 0.75;
  }
  35% {
    transform: none;
    opacity: 1;
  }
  52% {
    transform: none;
    opacity: 1;
  }
  55% {
    transform: skew(-1deg, 0.2deg);
    opacity: 0.75;
  }
  50% {
    transform: none;
    opacity: 1;
  }
  72% {
    transform: none;
    opacity: 1;
  }
  75% {
    transform: skew(0.4deg, 1deg);
    opacity: 0.75;
  }
  80% {
    transform: none;
    opacity: 1;
  }
  100% {
    transform: none;
    opacity: 1;
  }
}

  ${(props: { glitchy: boolean }) => { return props.glitchy ? 'animation: glitch1 2.5s infinite' : 'animation: none;'; }}

  @keyframes spin {
    from {
        transform:rotate(0deg);
    }
    to {
        transform:rotate(360deg);
    }
  }

  ${(props: { glitchy2: boolean }) => { return props.glitchy2 ? 'animation: spin 0.1s infinite;' : 'animation: none;'; }}
`;

const StyledEye = styled.div`
  background: url(${eye}) 0 0/contain no-repeat;
  height: 35em;
  position: absolute;
  top: 145em;
  width: 35em;

  ${(props: { asleep: boolean }) => {
    return props.asleep ? `
    animation: none;
    transform: scaleY(0.4);
  ` : `
  @keyframes eye-animation {
    0% {   transform: scaleY(1); }
    48% {   transform: scaleY(1); }
    53% {   transform: scaleY(0.6); }
    58% {   transform: scaleY(1); }
    100% {   transform: scaleY(1); }
  }

  animation-name: eye-animation;
  animation-duration: 5s;
  animation-iteration-count: infinite;
  `;
  }}
`
const LeftEye = styled(StyledEye)`
  left: 164em;
`;

const RightEye = styled(StyledEye)`
  right: 164em;
`;

const SantaBody = styled.div`
  @keyframes breathing {
    0% {   transform: translateY(0); }
    30% {   transform: translateY(-8em); }
    72% {   transform: translateY(0); }
    100% {   transform: translateY(0); }
  }

  background: url(${body}) 0 0/contain no-repeat;
  bottom: -580em;
  height: 1068em;
  position: absolute;
  width: 1052em;
  font-size: 0.8px;

@keyframes glitch1 {
  0% {
    transform: translateY(0);
    opacity: 1;
  }
  7% {
    transform: skew(-0.5deg, -0.9deg);
    opacity: 0.75;
  }
  10% {
    transform: none;
    opacity: 1;
  }
  27% {
    transform: none;
    opacity: 1;
  }
  30% {
    transform: skew(0.8deg, -0.1deg) translateY(-50em);
    opacity: 0.75;
  }
  35% {
    transform: none;
    opacity: 1;
  }
  52% {
    transform: none;
    opacity: 1;
  }
  55% {
    transform: skew(-1deg, 0.2deg);
    opacity: 0.75;
  }
  50% {
    transform: none;
    opacity: 1;
  }
  72% {
    transform: translateY(0);
    opacity: 1;
  }
  75% {
    transform: skew(0.4deg, 1deg);
    opacity: 0.75;
  }
  80% {
    transform: none;
    opacity: 1;
  }
  100% {
    transform: translateY(0);
    opacity: 1;
  }
}

  ${(props: { glitchy: boolean }) => { return props.glitchy ? 'animation: glitch1 2.5s infinite' : ''; }}
`;

const Eyebrow = styled.div`
  transform: translate3d(0em, 0em, 0px);
  background-color: #fff;
  border-radius: 8em;
  height: 22em;
  position: absolute;
  top: 95em;
  transform: translateZ(0);
  transition: transform .3s;
  width: 93em;
`;

const LeftEyebrow = styled(Eyebrow)`
  left: 132em;
  ${(props: { naughty: boolean }) => { return props.naughty ? 'transform: rotate(20deg);' : 'transform: rotate(0deg);'; }}
  ${(props: { glitchy: boolean }) => { return props.glitchy ? 'transform: rotate(30deg);' : 'transform: rotate(0deg);'; }}
`

const RightEyebrow = styled(Eyebrow)`
  right: 132em;
  ${(props: { naughty: boolean }) => { return props.naughty ? 'transform: rotate(-20deg);' : 'transform: rotate(0deg);'; }}
  ${(props: { glitchy: boolean }) => { return props.glitchy ? 'transform: rotate(-30deg);' : 'transform: rotate(0deg);'; }}
`

const Nose = styled.div`
  background: url(${nose}) 0 0/contain no-repeat;
  height: 104em;
  left: 215.5em;
  position: absolute;
  top: 170em;
  transform: translateZ(0);
  transition: transform .3s;
  width: 127em;
`

const Mouth = styled.div`
  background: 0 0/contain no-repeat ${(props: { naughty: boolean }) => { return props.naughty ? `url(${eye})` : `url(${mouth})`; }};
  ${(props: { naughty: boolean }) => { return props.naughty ? `left: 255em;` : `left: 234em;`; }};
  height: 45em;
  width: 90em;
  pointer-events: none;
  position: absolute;
  top: 295em;
  transform: translateZ(0);
`

const Beard = styled.img`
  left: 50%;
  position: absolute;
  top: 350px;
  transform: translate(-50%, -50%);
  aspect-ratio: auto 755 / 881;
`

const Hair = styled.img`
  position: absolute;
  width: 260px;
  left: 460px;
  top: 130px;

  @keyframes glitch1 {
  0% {
    transform: none;
    opacity: 1;
  }
  7% {
    transform: skew(-0.5deg, -0.9deg);
    opacity: 0.75;
  }
  10% {
    transform: none;
    opacity: 1;
  }
  27% {
    transform: none;
    opacity: 1;
  }
  30% {
    transform: skew(0.8deg, -0.1deg);
    opacity: 0.75;
  }
  35% {
    transform: none;
    opacity: 1;
  }
  52% {
    transform: none;
    opacity: 1;
  }
  55% {
    transform: skew(-1deg, 0.2deg);
    opacity: 0.75;
  }
  50% {
    transform: none;
    opacity: 1;
  }
  72% {
    transform: none;
    opacity: 1;
  }
  75% {
    transform: skew(0.4deg, 1deg);
    opacity: 0.75;
  }
  80% {
    transform: none;
    opacity: 1;
  }
  100% {
    transform: none;
    opacity: 1;
  }
}

  ${(props: { glitchy: boolean }) => { return props.glitchy ? 'animation: glitch1 1s infinite; left: 670px;' : ''; }}
`

// change whole element for naughty or nice so animation occurss every time text enters
const NaughtyOrNiceText = styled.div`
  @keyframes waviy {
    0% {
      font-size: 0px;
    }
    100% {
      font-size: 250px;
    }
  }
  animation: waviy 700ms cubic-bezier(0.34, 1.56, 0.64, 1);
  animation-delay: calc(.1s * var(--i));
  position: relative;
  bottom: 365px;
  left: 200px;
  display: flex;
  width: 100%;
  justify-content: center;
  font-size: 250px;
  font-family: 'Christmas';
  ${(props: { naughtyOrNice: 'Naughty!' | 'Nice!' }) => { return props.naughtyOrNice ? '' : 'display:none;'; }}
  color: ${(props: { naughtyOrNice: 'Naughty!' | 'Nice!' }) => { return props.naughtyOrNice === 'Nice!' ? 'green' : 'red'; }};
`

const NiceText = styled(NaughtyOrNiceText)`
    ${(props: { naughtyOrNice: 'Naughty!' | 'Nice!' }) => { return props.naughtyOrNice === 'Naughty!' ? 'display:none;' : ''; }}
`

const NaughtyText = styled(NaughtyOrNiceText)`
  bottom: 380px;
  left: 60px;
  z-index: 100;
    ${(props: { naughtyOrNice: 'Naughty!' | 'Nice!' }) => { return props.naughtyOrNice === 'Nice!' ? 'display:none;' : ''; }}
`

const NextButton = styled.button`
  border-radius: 3px;
  background-color: gold;
  border-color: green;
  border-width: 4px;
  color: green;
  padding: 10px;
  font-family: 'Christmas';
  font-size: 50px;
  width: 500px;
`

const StartButton = styled.button`
  border-radius: 3px;
  background-color: gold;
  border-color: green;
  border-width: 4px;
  color: green;
  padding: 10px;
  font-family: 'Christmas';
  font-size: 50px;
  width: 500px;
`

// change whole element for naughty or nice so animation occurss every time text enters
const ThinkingText = styled.div`
  @keyframes waviy {
    0% {
      font-size: 0px;
    }
    100% {
      font-size: 250px;
    }
  }
  animation: waviy 700ms cubic-bezier(0.34, 1.56, 0.64, 1);
  animation-delay: calc(.1s * var(--i));
  position: relative;
  bottom: 350px;
  display: flex;
  width: 100%;
  justify-content: center;
  font-size: 250px;
  font-family: 'Christmas';
  color: black;
  left: 200px;

  .dot-elastic {
  position: relative;
  top: 180px;
  left: 30px;
  width: 10px;
  height: 10px;
  border-radius: 5px;
  background-color: black;
  color: black;
  animation: dotElastic 1s infinite linear;
}

.dot-elastic::before, .dot-elastic::after {
  content: '';
  display: inline-block;
  position: absolute;
  top: 0;
}

.dot-elastic::before {
  left: -15px;
  width: 10px;
  height: 10px;
  border-radius: 5px;
  background-color: black;
  color: black;
  animation: dotElasticBefore 1s infinite linear;
}

.dot-elastic::after {
  left: 15px;
  width: 10px;
  height: 10px;
  border-radius: 5px;
  background-color: black;
  color: black;
  animation: dotElasticAfter 1s infinite linear;
}

@keyframes dotElasticBefore {
  0% {
    transform: scale(1, 1);
  }
  25% {
    transform: scale(1, 1.5);
  }
  50% {
    transform: scale(1, 0.67);
  }
  75% {
    transform: scale(1, 1);
  }
  100% {
    transform: scale(1, 1);
  }
}

@keyframes dotElastic {
  0% {
    transform: scale(1, 1);
  }
  25% {
    transform: scale(1, 1);
  }
  50% {
    transform: scale(1, 1.5);
  }
  75% {
    transform: scale(1, 1);
  }
  100% {
    transform: scale(1, 1);
  }
}

@keyframes dotElasticAfter {
  0% {
    transform: scale(1, 1);
  }
  25% {
    transform: scale(1, 1);
  }
  50% {
    transform: scale(1, 0.67);
  }
  75% {
    transform: scale(1, 1.5);
  }
  100% {
    transform: scale(1, 1);
  }
}
`

const Hal = styled.div`
  background: url(${hal}) 0 0/contain no-repeat;
  width: 3000px;
  height: 1000px;
  background-color: black;
`

type SantaState = {
  displayName?: boolean,
  displayNamesList?: boolean,
  namesLeftToDisplay: string[],
  mood: 'asleep' | 'ambient' | 'thinking' | 'glitchy' | 'glitchy2' | 'Naughty!' | 'Nice!' | 'hal' //merge with naughty or nice
}

function App() {
  const THINKING_TIME = 4000;
  // https://socket.io/docs/v4/client-initialization/
  const socket = io('localhost:8081'); // pass url when not on same domain

  const [santaState, setSantaState] = useState<SantaState>({
    displayNamesList: false,
    namesLeftToDisplay: ['Andy', 'Anton', 'Julie'],
    mood: 'asleep' // should be asleep
  });

  useEffect(() => {
    function handleMessage4() {
      const newArray = santaState.namesLeftToDisplay.filter((v, i) => i !== 0);
      setSantaState(s => ({ ...s, displayNamesList: false, mood: 'thinking', namesLeftToDisplay: newArray }));
      // const url = '<sb ip address>:8000/print';
      if (newArray.length === 0) {
        setTimeout(() => {
          setSantaState(s => ({ ...s, mood: 'Naughty!', displayNamesList: false, namesLeftToDisplay: newArray }));
          // todo url
          // fetch(url, {
          //   method: 'POST',
          //   body: 'naughty Julie',
          //   headers: {
          //     'Content-Type': 'text/plain'
          //   }
          // })
          setTimeout(() => {
            setSantaState(s => ({ ...s, mood: 'glitchy2', displayNamesList: false, namesLeftToDisplay: newArray }));
            setTimeout(() => {
              setSantaState(s => ({ ...s, mood: 'glitchy', displayNamesList: false, namesLeftToDisplay: newArray }));
              // setTimeout(() => {
              //   setSantaState(s => ({ ...s, mood: 'hal', displayNamesList: false, namesLeftToDisplay: newArray }));
              // }, 2000)
            }, 5000)
          }, 3000)
        }, THINKING_TIME + 3000)
      } else {
        setTimeout(() => {
          setSantaState(s => ({ ...s, displayNamesList: false, mood: 'Nice!', namesLeftToDisplay: newArray }));
  
          // todo url
          // fetch(url, {
          //   method: 'POST',
          //   body: `nice ${nameToDisplay}`,
          //   headers: {
          //     'Content-Type': 'text/plain'
          //   }
          // })
        }, THINKING_TIME + 1000)
      }
    }

    function onStatusChange(msg: string) {
      console.log('message ', msg);
      if (msg === 'touched') {
        // console.log('message is 4 ', nameToDisplay);
        handleMessage4();
      }

      if (msg === 'hal') {
        // console.log('message is 4 ', nameToDisplay);
        setSantaState(s => ({ ...s, mood: 'hal' }))
      }
    }

    socket.on('stateChange', onStatusChange);
    return () => { socket.removeListener('stateChange', onStatusChange) };
  },[socket, santaState.namesLeftToDisplay])
  // socket.on('stateChange', function (msg) {
  //   console.log('STATUS CHANGE')
  //   if (msg === 4) {
  //     // console.log('message is 4 ', nameToDisplay);
  //     handleMessage4();
  //   }
  // });
  // useEffect(() => {
  //   setSantaState({ ...santaState,  mood: 'ambient' })
  // }, []);
  // useEffect(() => {
  //   if (santaState.displayNamesList) {
  //     const prevNameI = santaState.namesLeftToDisplay.indexOf(nameToDisplay);
  //     const newNameToDisplay = prevNameI === -1 ? santaState.namesLeftToDisplay[0] : santaState.namesLeftToDisplay[prevNameI + 1];

  //     setNameToDisplay(newNameToDisplay);
  //   }
  // }, [santaState.displayNamesList, santaState.namesLeftToDisplay]);
  // console.log('name to display ', nameToDisplay);
  console.log('names left to display ', santaState.namesLeftToDisplay);

  return (
    santaState.mood === 'hal' ? <Hal /> : <PageContainer glitchy={santaState.mood === 'glitchy'} glitchy2={santaState.mood === 'glitchy2'}>
      <Page glitchy={santaState.mood === 'glitchy'} glitchy2={santaState.mood === 'glitchy2'}>
        {santaState.mood === 'asleep' && <StartButton onClick={() => { fetch('http://192.168.1.195/setState', {
    method: 'POST',
    body: '1',
    headers: {
      'content-type': 'text/plain'
    }
  }); setTimeout(() => { setSantaState(s => ({ ...s, displayNamesList: true, mood: 'ambient' })); }, 5000); }}>Start Santabot</StartButton>}
        {santaState.displayNamesList && <Names name={santaState.namesLeftToDisplay[0]} />}
        {santaState.mood === 'asleep' && <Sleeping />}
        {santaState.mood === 'thinking' && <ThinkingText>Thinking<div className="dot-elastic"></div></ThinkingText>}
        <NiceText naughtyOrNice={santaState.mood}>{santaState.mood === 'Nice!' ? 'Nice!' : ''}</NiceText>
        {santaState.mood === 'Nice!' && <NextButton onClick={() => setSantaState(s => ({ ...s, displayNamesList: true, mood: 'ambient' }))}>Continue Evaluation</NextButton>}
        <NaughtyText naughtyOrNice={santaState.mood}>{santaState.mood === 'Naughty!' ? 'Naughty!' : ''}</NaughtyText>
        <SantaBody data-id="body" glitchy={santaState.mood === 'glitchy' || santaState.mood === 'glitchy2'} />
        <SantaFace data-id="face" glitchy={santaState.mood === 'glitchy'} glitchy2={santaState.mood === 'glitchy2'}>
          <LeftEye data-id="left-eye" asleep={santaState.mood === 'asleep'} />
          <RightEye data-id="right-eye" asleep={santaState.mood === 'asleep'} />
          <LeftEyebrow data-id="left-eyebrow" naughty={santaState.mood === 'Naughty!'} glitchy={santaState.mood === 'glitchy'} />
          <RightEyebrow data-id="right-eyebrow" naughty={santaState.mood === 'Naughty!'} glitchy={santaState.mood === 'glitchy'} />
          <Beard src={beard} alt="Beard" />
          <Nose data-id="nose" />
          <Mouth naughty={santaState.mood === 'Naughty!'} />
        </SantaFace>
        <Hair src={hair} alt="Hair" glitchy={santaState.mood === 'glitchy'} />
      </Page>
    </PageContainer>
  );
}

export default App;
