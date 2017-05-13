// Written by http://xojoc.pw. Public domain.

package main

import (
	"image"
	"image/color"
	"math"

	"gitlab.com/xojoc/util"
)

const width = 500
const height = 500

var canvas = image.NewRGBA(image.Rect(0, 0, width, height))

func main() {
	for x := 0; x < width; x++ {
		xₙ := float64(x) / width
		for i := 0; i < 1e7; i++ {
			canvas.Set(x, int(height*xₙ+0.5), color.RGBA{0x0, 0x0, 0xa0, 0xff})
			xₙ = math.Mod(2*xₙ, 1)
		}
	}
	util.MustWriteImage("dyadic_map.png", canvas)
}
