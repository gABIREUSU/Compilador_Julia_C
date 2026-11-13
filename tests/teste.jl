julia
# ============================================
# Arquivo para teste de analisador léxico
# Linguagem: Julia
# ============================================

# Palavras-chave e identificadores
function calcular_media(notas)
    total = 0
    quantidade = length(notas)

    for nota in notas
        total += nota
    end

    if quantidade > 0
        media = total / quantidade
    else
        media = 0.0
    end

    return media
end

# Tipos numéricos
idade = 21
altura = 1.75
temperatura = -10.5
hexadecimal = 0xFF
binario = 0b101010
octal = 0o755
cientifico = 6.02e23

# Booleanos
aprovado = true
reprovado = false

# Caracteres e strings
letra = 'A'
simbolo = '$'
nome = "Gabriel"
mensagem = "Olá, mundo!"
texto_multilinha = """
Esta é uma string
com várias linhas.
"""

# Arrays
numeros = [1, 2, 3, 4, 5]
nomes = ["Ana", "Bruno", "Carlos"]
matriz = [1 2 3; 4 5 6; 7 8 9]

# Tuplas
pessoa = ("Gabriel", 21)
coordenadas = (10.5, 20.3)

# Operadores aritméticos
a = 10
b = 3

soma = a + b
subtracao = a - b
multiplicacao = a * b
divisao = a / b
divisao_inteira = a ÷ b
resto = a % b
potencia = a ^ b

# Operadores de comparação
igual = a == b
diferente = a != b
menor = a < b
maior = a > b
menor_igual = a <= b
maior_igual = a >= b

# Operadores lógicos
resultado1 = (a > 5) && (b < 10)
resultado2 = (a > 5) || (b > 10)
resultado3 = !aprovado

# Operadores de atribuição
x = 10
x += 5
x -= 2
x *= 3
x /= 2

# Operadores bit a bit
bits_and = 0b1100 & 0b1010
bits_or = 0b1100 | 0b1010
bits_xor = 0b1100 ⊻ 0b1010
bits_not = ~0b1100

# Condicionais
if idade >= 18
    println("Maior de idade")
elseif idade >= 12
    println("Adolescente")
else
    println("Criança")
end

# Laços
for i in 1:10
    println(i)
end

contador = 0

while contador < 5
    contador += 1
end

# Break e continue
for i in 1:20
    if i == 10
        break
    end

    if i % 2 == 0
        continue
    end

    println(i)
end

# Função com múltiplos parâmetros
function soma(x, y)
    return x + y
end

# Função anônima
dobro = x -> x * 2

# Estrutura
struct Pessoa
    nome::String
    idade::Int
end

usuario = Pessoa("João", 25)

# Símbolos e tipos
tipo = Int
tipo_string = String
nada = nothing
ausente = missing

# Operadores de intervalo
intervalo = 1:10
passo = 1:2:10

# Broadcasting
valores = [1, 2, 3]
resultado = valores .* 2
resultado2 = valores .+ 10

# Operador ternário
status = idade >= 18 ? "adulto" : "menor"

# Operador de composição
f = x -> x * 2
g = x -> x + 1
h = f ∘ g

# Comentário de linha
valor = 100 # comentário depois de código

# Comentário de bloco
#= 
   Este é um comentário
   de múltiplas linhas.
=#

# Delimitadores
lista = [1, 2, 3]
tupla = (1, 2, 3)
dicionario = Dict("a" => 1, "b" => 2)

# Interpolação de strings
usuario_nome = "Gabriel"
saudacao = "Olá, $usuario_nome!"
calculo = "2 + 2 = $(2 + 2)"

println(saudacao)
println(calculo)