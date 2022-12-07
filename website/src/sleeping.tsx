import { useEffect, useState } from 'react';
import styled from 'styled-components';
import './sleeping.css';

function Sleeping() {
    return (
        <>
            <div id="pillow">
                <div className="zzz zzz-zzz">Z</div>
                <div className="zzz zzz-zz">Z</div>
                <div className="zzz zzz-z">Z</div>
                <div className="corner top-left"></div>
                <div className="corner top-right"></div>
                <div className="corner bottom-right"></div>
                <div className="corner bottom-left"></div>
            </div>
        </>
    );
}

export default Sleeping;
