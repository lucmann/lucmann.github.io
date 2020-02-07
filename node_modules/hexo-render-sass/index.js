/* global hexo */ "use strict";



//  P A C K A G E

const renderer = require("./lib/renderer");



//  E X P O R T S

hexo.extend.renderer.register("scss", "css", renderer);
hexo.extend.renderer.register("sass", "css", renderer);
