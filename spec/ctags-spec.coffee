ctags = require '../lib/ctags'
path = require 'path'

describe 'ctags', ->
  tagsFile = null

  beforeEach ->
    tagsFile = path.join(__dirname, 'fixtures/tags')

  describe '.findTag(name)', ->
    it 'returns all matching tags', ->
      tags = ctags.findTag(tagsFile, 'duplicate')
      expect(tags.length).toBe 2

      expect(tags[0].file).toBe 'tagged-duplicate.js'
      expect(tags[0].name).toBe 'duplicate'
      expect(tags[0].pattern).toBe '/^function duplicate() {$/'

      expect(tags[1].file).toBe 'tagged.js'
      expect(tags[1].name).toBe 'duplicate'
      expect(tags[1].pattern).toBe '/^function duplicate() {$/'

    describe 'when partialMatch is set to true', ->
      it 'returns tags that partially match the name', ->
        tags = ctags.findTag(tagsFile, 'dup', partialMatch: true)
        expect(tags.length).toBe 2

        expect(tags[0].file).toBe 'tagged-duplicate.js'
        expect(tags[0].name).toBe 'duplicate'
        expect(tags[0].pattern).toBe '/^function duplicate() {$/'

        expect(tags[1].file).toBe 'tagged.js'
        expect(tags[1].name).toBe 'duplicate'
        expect(tags[1].pattern).toBe '/^function duplicate() {$/'

  describe '.getTags()', ->
    it 'returns all tags', ->
      tags = ctags.getTags(tagsFile)
      expect(tags.length).toBe 4

      expect(tags[0].file).toBe 'tagged.js'
      expect(tags[0].name).toBe 'callMeMaybe'
      expect(tags[0].pattern).toBe '/^function callMeMaybe() {$/'

      expect(tags[1].file).toBe 'tagged-duplicate.js'
      expect(tags[1].name).toBe 'duplicate'
      expect(tags[1].pattern).toBe '/^function duplicate() {$/'

      expect(tags[2].file).toBe 'tagged.js'
      expect(tags[2].name).toBe 'duplicate'
      expect(tags[2].pattern).toBe '/^function duplicate() {$/'

      expect(tags[3].file).toBe 'tagged.js'
      expect(tags[3].name).toBe 'thisIsCrazy'
      expect(tags[3].pattern).toBe '/^var thisIsCrazy = true;$/'
