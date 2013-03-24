# Ctags Node module [![Build Status](https://travis-ci.org/atom/node-ctags.png)](https://travis-ci.org/atom/node-ctags)

Read all about ctags [here](http://ctags.sourceforge.net/).

## Installing

```sh
npm install ctags
```

## Building
  * Clone the repository
  * Run `npm install`
  * Run `grunt` to compile the native and CoffeeScript code

## Documentation

### findTags(path, tag, [options])

Get all tags matching the tag specified from the tags file at the path.

The following `options` are supported:
  * `caseInsensitive`: Include tags that match case insensitively,
    defaults to `false`
  * `partialMatch`: Include tags that partially match, defaults to `false`

```coffeescript
ctags = require 'ctags'

tags = ctags.findTags('/Users/me/repos/node/tags', 'exists')
for tag in tags
  console.log("#{tag.name} is in #{tag.file}")
```

### getTags(path)

Get all tags found in the path specified.

```coffeescript
ctags = require 'ctags'

allTags = ctags.getTags('/Users/me/repos/node/tags')
for tag in allTags
  console.log("#{tag.name} is in #{tag.file} with pattern: #{tag.pattern}")
```
