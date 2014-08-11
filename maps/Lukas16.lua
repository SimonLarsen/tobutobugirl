return {
  version = "1.1",
  luaversion = "5.1",
  orientation = "orthogonal",
  width = 10,
  height = 16,
  tilewidth = 16,
  tileheight = 16,
  properties = {},
  tilesets = {
    {
      name = "tiles",
      firstgid = 1,
      tilewidth = 16,
      tileheight = 16,
      spacing = 0,
      margin = 0,
      image = "../tiles.png",
      imagewidth = 16,
      imageheight = 176,
      properties = {},
      tiles = {}
    }
  },
  layers = {
    {
      type = "tilelayer",
      name = "Tile Layer 1",
      x = 0,
      y = 0,
      width = 10,
      height = 16,
      visible = true,
      opacity = 1,
      properties = {},
      encoding = "lua",
      data = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
      }
    },
    {
      type = "objectgroup",
      name = "Object Layer 1",
      visible = true,
      opacity = 1,
      properties = {},
      objects = {
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 16,
          y = 96,
          width = 0,
          height = 0,
          gid = 6,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 16,
          y = 80,
          width = 0,
          height = 0,
          gid = 8,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 32,
          y = 144,
          width = 0,
          height = 0,
          gid = 7,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 48,
          y = 192,
          width = 0,
          height = 0,
          gid = 10,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 48,
          y = 208,
          width = 0,
          height = 0,
          gid = 6,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 32,
          y = 256,
          width = 0,
          height = 0,
          gid = 2,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 128,
          y = 176,
          width = 0,
          height = 0,
          gid = 4,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 96,
          y = 128,
          width = 0,
          height = 0,
          gid = 7,
          visible = true,
          properties = {}
        },
        {
          name = "",
          type = "",
          shape = "rectangle",
          x = 128,
          y = 80,
          width = 0,
          height = 0,
          gid = 11,
          visible = true,
          properties = {}
        }
      }
    }
  }
}
