# OniLang

**OniLang** is an experimental scripting language designed for simplicity, readability, and ease of embedding. It’s implemented in C and interpreted at runtime. The syntax is inspired by modern high-level languages, but the runtime is intentionally minimal and educational.

> ⚠️ OniLang is **not production-safe**. Memory safety and performance optimizations are not guaranteed. It is a **work-in-progress** and welcomes contributions and ideas.

<p align="center">
<img src="https://github.com/alperakkin/oni-lang/blob/main/public/oni.jpg" style="display: block; margin: auto;self-align:center;" width="600">
</p>
---
## 🧙 About the Name
Oni (鬼) are mythological creatures from Japanese and Korean folklore, often depicted as horned ogres or demons. While traditionally seen as fearsome, they are also symbolic of inner strength and transformation. OniLang takes inspiration from this concept — a small but mighty language, simple on the outside but capable of evolving and growing with contributions from the community.

## 🚀 Examples

### Function Definition and Call

```oni
@sum(int a, int b=2) <int>
{
    print("calculating..")
    return a + b
}

int z = sum(2, b=4)
print(z)

print(sum(1))
```

- `@sum` defines a function with typed arguments.
- Default values are supported.
- `<int>` defines the return type (optional).

---

### Basic Function Usage

```oni
@greet(str name)
{
    print(name)
}

greet("John Doe")
```

- Functions can accept positional arguments.
- Return type can be omitted (defaults to `null`).

---

### Variable and Array Declarations

```oni
int x = 10
str message = "Hello"
arr<int> numbers = [1, 2, 3]
```

- Supports primitive types (`int`, `str`) and typed arrays (`arr<int>`).
- Arrays are literal-defined with brackets `[]`.

---

### Returning Values

```oni
@multiply(int a, int b) <int>
{
    return a * b
}

print(multiply(3, 4))
```

---

## ⚙️ Building and Running

```bash
make
./bin/oni examples/example.oni
```

---

## 📁 Example File

```oni
@greet(str name)
{
    print("Hello, " + name)
}

@goodbye()
{
    print("Goodbye!")
}

greet("OniLang")
goodbye()
```

---

## 🧪 Testing

You can run all example scripts in the `examples/` directory:

```bash
./scripts/run.sh
```

Each script is compiled and interpreted individually.

---

## ⚠️ Disclaimer

OniLang is:

- **Not memory safe**: It does not implement memory protection or garbage collection.
- **Not optimized**: Performance is not a priority.
- **Not production-ready**: It is a prototype language designed for experimentation and learning.

---

## 🤝 Collaboration

Feel free to open issues, report bugs, or suggest features. All contributions are welcome, whether it's improving parsing, building a REPL, adding standard library functions, or optimizing memory usage.

---

## 📄 License
GPL-3.0


