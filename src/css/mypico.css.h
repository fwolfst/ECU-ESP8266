#ifndef _MYPICO_CSS_H
#define _MYPICO_CSS_H

/** Created from picocss, see README.md */
const char MYPICO_CSS[] PROGMEM = R"=====(
:root{--font-family:system-ui,-apple-system,"Segoe UI","Roboto","Ubuntu","Cantarell","Noto Sans",sans-serif,"Apple Color Emoji","Segoe UI Emoji","Segoe UI Symbol","Noto Color Emoji";--line-height:1.5;--font-weight:400;--font-size:16px;--border-radius:.25rem;--border-width:1px;--outline-width:3px;--spacing:1rem;--typography-spacing-vertical:1.5rem;--block-spacing-vertical:calc(var(--spacing) * 2);--block-spacing-horizontal:var(--spacing);--form-element-spacing-vertical:.75rem;--form-element-spacing-horizontal:1rem;--nav-element-spacing-vertical:1rem;--nav-element-spacing-horizontal:.5rem;--nav-link-spacing-vertical:.5rem;--nav-link-spacing-horizontal:.5rem;--form-label-font-weight:var(--font-weight);--transition:.2s ease-in-out;--modal-overlay-backdrop-filter:blur(0.25rem)}@media(min-width:576px){:root{--font-size:17px}}@media(min-width:768px){:root{--font-size:18px}}@media(min-width:992px){:root{--font-size:19px}}@media(min-width:1200px){:root{--font-size:20px}}@media(min-width:576px){body>header,body>main,body>footer,section{--block-spacing-vertical:calc(var(--spacing) * 2.5)}}@media(min-width:768px){body>header,body>main,body>footer,section{--block-spacing-vertical:calc(var(--spacing) * 3)}}@media(min-width:992px){body>header,body>main,body>footer,section{--block-spacing-vertical:calc(var(--spacing) * 3.5)}}@media(min-width:1200px){body>header,body>main,body>footer,section{--block-spacing-vertical:calc(var(--spacing) * 4)}}@media(min-width:576px){article{--block-spacing-horizontal:calc(var(--spacing) * 1.25)}}@media(min-width:768px){article{--block-spacing-horizontal:calc(var(--spacing) * 1.5)}}@media(min-width:992px){article{--block-spacing-horizontal:calc(var(--spacing) * 1.75)}}@media(min-width:1200px){article{--block-spacing-horizontal:calc(var(--spacing) * 2)}}dialog>article{--block-spacing-vertical:calc(var(--spacing) * 2);--block-spacing-horizontal:var(--spacing)}@media(min-width:576px){dialog>article{--block-spacing-vertical:calc(var(--spacing) * 2.5);--block-spacing-horizontal:calc(var(--spacing) * 1.25)}}@media(min-width:768px){dialog>article{--block-spacing-vertical:calc(var(--spacing) * 3);--block-spacing-horizontal:calc(var(--spacing) * 1.5)}}a{--text-decoration:none}small{--font-size:.875em}h1,h2,h3,h4,h5,h6{--font-weight:700}h1{--font-size:2rem;--typography-spacing-vertical:3rem}h2{--font-size:1.75rem;--typography-spacing-vertical:2.625rem}h3{--font-size:1.5rem;--typography-spacing-vertical:2.25rem}h4{--font-size:1.25rem;--typography-spacing-vertical:1.874rem}h5{--font-size:1.125rem;--typography-spacing-vertical:1.6875rem}[type="checkbox"],[type="radio"]{--border-width:2px}[type="checkbox"][role="switch"]{--border-width:3px}thead th,thead td,tfoot th,tfoot td{--border-width:3px}:not(thead,tfoot)>*>td{--font-size:.875em}pre,code,kbd,samp{--font-family:"Menlo","Consolas","Roboto Mono","Ubuntu Monospace","Noto Mono","Oxygen Mono","Liberation Mono",monospace,"Apple Color Emoji","Segoe UI Emoji","Segoe UI Symbol","Noto Color Emoji"}kbd{--font-weight:bolder}*,*::before,*::after{box-sizing:border-box;background-repeat:no-repeat}::before,::after{text-decoration:inherit;vertical-align:inherit}:where(:root){-webkit-tap-highlight-color:transparent;-webkit-text-size-adjust:100%;text-size-adjust:100%;background-color:var(--background-color);color:var(--color);font-weight:var(--font-weight);font-size:var(--font-size);line-height:var(--line-height);font-family:var(--font-family);text-rendering:optimizeLegibility;overflow-wrap:break-word;cursor:default;tab-size:4}main{display:block}body{width:100%;margin:0}body>header,body>main,body>footer{width:100%;margin-right:auto;margin-left:auto;padding:var(--block-spacing-vertical) var(--block-spacing-horizontal)}:where(nav li)::before{float:left;content:"\200B"}nav,nav ul{display:flex}nav{justify-content:space-between}nav ol,nav ul{align-items:center;margin-bottom:0;padding:0;list-style:none}nav ol:first-of-type,nav ul:first-of-type{margin-left:calc(var(--nav-element-spacing-horizontal) * -1)}nav ol:last-of-type,nav ul:last-of-type{margin-right:calc(var(--nav-element-spacing-horizontal) * -1)}nav li{display:inline-block;margin:0;padding:var(--nav-element-spacing-vertical) var(--nav-element-spacing-horizontal)}nav li>*{--spacing:0}nav :where(a,[role="link"]){display:inline-block;margin:calc(var(--nav-link-spacing-vertical) * -1) calc(var(--nav-link-spacing-horizontal) * -1);padding:var(--nav-link-spacing-vertical) var(--nav-link-spacing-horizontal);border-radius:var(--border-radius);text-decoration:none}nav :where(a,[role="link"]):is([aria-current],:hover,:active,:focus){text-decoration:none}nav[aria-label="breadcrumb"]{align-items:center;justify-content:start}nav[aria-label="breadcrumb"] ul li:not(:first-child){margin-inline-start:var(--nav-link-spacing-horizontal)}nav[aria-label="breadcrumb"] ul li:not(:last-child) ::after{position:absolute;width:calc(var(--nav-link-spacing-horizontal) * 2);margin-inline-start:calc(var(--nav-link-spacing-horizontal) / 2);content:"/";color:var(--muted-color);text-align:center}nav[aria-label="breadcrumb"] a[aria-current]{background-color:transparent;color:inherit;text-decoration:none;pointer-events:none}nav [role="button"]{margin-right:inherit;margin-left:inherit;padding:var(--nav-link-spacing-vertical) var(--nav-link-spacing-horizontal)}aside nav,aside ol,aside ul,aside li{display:block}aside li{padding:calc(var(--nav-element-spacing-vertical) * .5) var(--nav-element-spacing-horizontal)}aside li a{display:block}aside li [role="button"]{margin:inherit}[dir="rtl"] nav[aria-label="breadcrumb"] ul li:not(:last-child) ::after{content:"\\"}button{margin:0;overflow:visible;font-family:inherit;text-transform:none}button,[type="button"],[type="reset"],[type="submit"]{-webkit-appearance:button}button{display:block;width:100%;margin-bottom:var(--spacing)}[role="button"]{display:inline-block;text-decoration:none}button,input[type="submit"],input[type="button"],input[type="reset"],[role="button"]{--background-color:var(--primary);--border-color:var(--primary);--color:var(--primary-inverse);--box-shadow:var(--button-box-shadow,0 0 0 transparent);padding:var(--form-element-spacing-vertical) var(--form-element-spacing-horizontal);border:var(--border-width) solid var(--border-color);border-radius:var(--border-radius);outline:0;background-color:var(--background-color);box-shadow:var(--box-shadow);color:var(--color);font-weight:var(--font-weight);font-size:1rem;line-height:var(--line-height);text-align:center;cursor:pointer;transition:background-color var(--transition),border-color var(--transition),color var(--transition),box-shadow var(--transition)}button:is([aria-current],:hover,:active,:focus),input[type="submit"]:is([aria-current],:hover,:active,:focus),input[type="button"]:is([aria-current],:hover,:active,:focus),input[type="reset"]:is([aria-current],:hover,:active,:focus),[role="button"]:is([aria-current],:hover,:active,:focus){--background-color:var(--primary-hover);--border-color:var(--primary-hover);--box-shadow:var(--button-hover-box-shadow,0 0 0 transparent);--color:var(--primary-inverse)}button:focus,input[type="submit"]:focus,input[type="button"]:focus,input[type="reset"]:focus,[role="button"]:focus{--box-shadow:var(--button-hover-box-shadow,0 0 0 transparent),0 0 0 var(--outline-width) var(--primary-focus)}input[type="reset"]{--background-color:var(--secondary);--border-color:var(--secondary);--color:var(--secondary-inverse);cursor:pointer}input[type="reset"]:is([aria-current],:hover,:active,:focus){--background-color:var(--secondary-hover);--border-color:var(--secondary-hover)}input[type="reset"]:focus{--box-shadow:var(--button-hover-box-shadow,0 0 0 transparent),0 0 0 var(--outline-width) var(--secondary-focus)}:where(button,[type="submit"],[type="button"],[type="reset"],[role="button"])[disabled],:where(fieldset[disabled]) :is(button,[type="submit"],[type="button"],[type="reset"],[role="button"]),a[role="button"]:not([href]){opacity:.5;pointer-events:none}b,strong{font-weight:bolder}sub,sup{position:relative;font-size:.75em;line-height:0;vertical-align:baseline}sub{bottom:-0.25em}sup{top:-0.5em}address,blockquote,dl,figure,form,ol,p,pre,table,ul{margin-top:0;margin-bottom:var(--typography-spacing-vertical);color:var(--color);font-style:normal;font-weight:var(--font-weight);font-size:var(--font-size)}a,[role="link"]{--color:var(--primary);--background-color:transparent;outline:0;background-color:var(--background-color);color:var(--color);text-decoration:var(--text-decoration);transition:background-color var(--transition),color var(--transition),text-decoration var(--transition),box-shadow var(--transition)}a:is([aria-current],:hover,:active,:focus),[role="link"]:is([aria-current],:hover,:active,:focus){--color:var(--primary-hover);--text-decoration:underline}a:focus,[role="link"]:focus{--background-color:var(--primary-focus)}h1,h2,h3,h4,h5,h6{margin-top:0;margin-bottom:var(--typography-spacing-vertical);color:var(--color);font-weight:var(--font-weight);font-size:var(--font-size);font-family:var(--font-family)}h1{--color:var(--h1-color)}h2{--color:var(--h2-color)}h3{--color:var(--h3-color)}h4{--color:var(--h4-color)}h5{--color:var(--h5-color)}h6{--color:var(--h6-color)}:where(address,blockquote,dl,figure,form,ol,p,pre,table,ul) ~ :is(h1,h2,h3,h4,h5,h6){margin-top:var(--typography-spacing-vertical)}hgroup{margin-bottom:var(--typography-spacing-vertical)}hgroup>*{margin-bottom:0}hgroup>*:last-child{--color:var(--muted-color);--font-weight:unset;font-size:1rem;font-family:unset}p{margin-bottom:var(--typography-spacing-vertical)}small{font-size:var(--font-size)}:where(dl,ol,ul){padding-right:0;padding-left:var(--spacing);padding-inline-start:var(--spacing);padding-inline-end:0}:where(dl,ol,ul) li{margin-bottom:calc(var(--typography-spacing-vertical) * .25)}:where(dl,ol,ul) :is(dl,ol,ul){margin:0;margin-top:calc(var(--typography-spacing-vertical) * .25)}ul li{list-style:square}mark{padding:.125rem .25rem;background-color:var(--mark-background-color);color:var(--mark-color);vertical-align:baseline}blockquote{display:block;margin:var(--typography-spacing-vertical) 0;padding:var(--spacing);border-right:0;border-left:.25rem solid var(--blockquote-border-color);border-inline-start:.25rem solid var(--blockquote-border-color);border-inline-end:none}blockquote footer{margin-top:calc(var(--typography-spacing-vertical) * .5);color:var(--blockquote-footer-color)}abbr[title]{border-bottom:1px dotted;text-decoration:none;cursor:help}ins{color:var(--ins-color);text-decoration:none}del{color:var(--del-color)}::selection{background-color:var(--primary-focus)}
)=====";

#endif