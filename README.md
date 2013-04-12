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
  * Run `grunt test` to run the specs

## Documentation

### findTags(path, tag, [options])

Get all tags matching the tag specified from the tags file at the path.

`path` - The string path to the tags file.

`tag` - The string name of the tag to search for.

`options` - An optional options object containing the following keys:
  * `caseInsensitive`: `true` to include tags that match case insensitively,
    (default: `false`)
  * `partialMatch`: `true` to include tags that partially match the given tag
    (default: `false`)

Returns an array containing objects that have `name` and `file` keys and
optionally a `pattern` key if the tag file specified contains tag patterns.

#### Example

```coffeescript
ctags = require 'ctags'

tags = ctags.findTags('/Users/me/repos/node/tags', 'exists')
for tag in tags
  console.log("#{tag.name} is in #{tag.file}")
```

### getTags(path)

Get all tags found in the path specified.

`path` - The string path to the tags file.

Returns an array containing objects that have `name` and `file` keys and
optionally a `pattern` key if the tag file specified contains tag patterns.

#### Example

```coffeescript
ctags = require 'ctags'

allTags = ctags.getTags('/Users/me/repos/node/tags')
for tag in allTags
  console.log("#{tag.name} is in #{tag.file} with pattern: #{tag.pattern}")
```
