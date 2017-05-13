// Written by http://xojoc.pw. Public domain.

package main

import (
	"image"
	"image/color"
	"log"
	"math"

	"github.com/lucasb-eyer/go-colorful"
	"gitlab.com/xojoc/util"
)

const π = math.Pi

var sin = math.Sin

const width = 300
const height = 300

var canvas = image.NewRGBA(image.Rect(0, 0, width, height))

func θn1(θₙ, Ω, K float64) float64 {
	return θₙ + Ω - (K/(2*π))*sin(2*π*θₙ)
}

func ω(Ω, K float64) float64 {
	θₙ := 0.0
	n := 2000.0
	for i := 0.0; i < n; i++ {
		θₙ = θn1(θₙ, Ω, K)
	}
	return θₙ / n
}

func col(ω float64) color.Color {
	black := colorful.LinearRgb(0, 0, 0)
	green := colorful.LinearRgb(0, 1, 0)
	red := colorful.LinearRgb(1, 0, 0)
	if ω < 1.0/2 {
		return black.BlendLab(green, ω*2).Clamped()
	} else {
		if ω > 1 {
			log.Println("one ", ω)
			ω = 1
		}
		ω = (ω - 0.5) * 2
		if ω > 1 {
			log.Println("2 ", ω)
		}
		return green.BlendLab(red, ω).Clamped()
	}
}

func main() {
	for y := 0; y < height; y++ {
		for x := 0; x < width; x++ {
			Ω := float64(x) / width
			K := 2 * π * float64(height-y) / height
			canvas.Set(x, y, col(ω(Ω, K)))
		}
	}
	util.MustWriteImage("circle_map.png", canvas)
}
