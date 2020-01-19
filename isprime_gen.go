package main

import (
	"fmt"

	"github.com/notnil/combos"
)

func isprime(n int) bool {
	for i := 2; i < n; i++ {
		if n%i == 0 {
			return false
		}
	}
	return true
}

var operators = []string{
	//	"-",
	//	"+", "/",
	//	"%",
	//	"*",
	"&", "|", "^", "<<", ">>"}
var constants = []int{
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
	11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
	31, 32,

	33, 34, 35, 36, 37, 38, 39, 40,

	32, 64, 128,

	256, 512, 1024, 2048, 4196,
}

func evaluate(p int, o string, n int) int {
	switch o {
	case "-":
		p -= n
	case "+":
		p += n
	case "/":
		p /= n
	case "%":
		p %= n
	case "*":
		p *= n
	case "&":
		p &= n
	case "|":
		p |= n
	case "^":
		p ^= n
	case "<<":
		p <<= uint(n)
	case ">>":
		p >>= uint(n)
	default:
		panic("unknown operator: " + o)
	}
	return p
}

func result(ops, cons []int, p int) int {
	for i, op := range ops {
		p = evaluate(p, operators[op], constants[cons[i]])
	}
	return p
}

func test(ops []int, cons []int) bool {
	var ps = []int{3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199,

		617, 619,

		1039, 1093,

		1279,
		859433,
		179424691,

		104677,

		104681,
		104683,
		104693,
		104701,
		104707,
		104711,
		104717,
		104723,
		104729,
	}

	samer := result(ops, cons, 104723)

	for _, p := range ps {
		if result(ops, cons, p) != samer {
			return false
		}
	}
	var nps = []int{4, 6, 8, 9, 10, 16, 1024}

	for _, np := range nps {
		if result(ops, cons, np) == samer {
			return false
		}
	}
	/*
		for p := 3; p < 10; p++ {
			if result(ops, cons, p) == 0 {
				fmt.Printf("p: %v, %v\n", p, isprime(p))
				if !isprime(p) {
					return false
				}
			}
		}
	*/

	return true
}

func constants_combinations(k int) chan []int {
	cons := make(chan []int, 1000)

	go func() {
		for _, combo := range combos.New(len(constants), k) {
			cons <- combo
		}
		close(cons)
	}()
	return cons
}

func operators_combinations() chan []int {
	ops := make(chan []int, 1000)

	go func() {
		for i := 1; i <= len(operators); i++ {
			for _, combo := range combos.New(len(operators), i) {
				ops <- combo
			}
		}
		close(ops)
	}()
	return ops
}

func print(ops, cons []int) {
	fmt.Printf("isprime: ")
	for i, op := range ops {
		fmt.Printf(" %v %v", operators[op], constants[cons[i]])
	}
	fmt.Println()
}

func main() {
	for ops := range operators_combinations() {
		for cons := range constants_combinations(len(ops)) {
			if test(ops, cons) {
				print(ops, cons)
			}

		}
	}
}
