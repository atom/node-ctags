# Ctags Node module [![Build Status](https://travis-ci.org/atom/node-ctags.png)](https://travis-ci.org/atom/node-ctags)

Read all about ctags [here](http://ctags.sourceforge.net/).

## Installing

```sh
npm install ctags
```

## Documentation

### findTag(path, tag)

Get all tags matching the tag specified from the tags file at the path.

```coffeescript
ctags = require 'ctags'

tags = ctags.findTag('/Users/me/repos/node/tags', 'exists')
```

### getTags(path)

Get all tags found in the path specified.

```coffeescript
ctags = require 'ctags'

allTags = ctags.getTags('/Users/me/repos/node/tags')
```
