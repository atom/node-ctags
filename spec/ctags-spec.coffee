ctags = require '../lib/ctags'
path = require 'path'

describe "ctags", ->
  tagsFile = null

  beforeEach ->
    tagsFile = path.join(__dirname, 'fixtures/tags')

  describe "find", ->
    it "returns all matching tags", ->
      tags = ctags.find(tagsFile, 'duplicate')
      expect(tags.length).toBe 2

      expect(tags[0].file).toBe 'tagged-duplicate.js'
      expect(tags[0].name).toBe 'duplicate'
      expect(tags[0].pattern).toBe '/^function duplicate() {$/'

      expect(tags[1].file).toBe 'tagged.js'
      expect(tags[1].name).toBe 'duplicate'
      expect(tags[1].pattern).toBe '/^function duplicate() {$/'
