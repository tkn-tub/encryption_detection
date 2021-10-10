tbtfunc <- function(input){
import("CryptRndTest")
import("broman")
y <- input
x <- vector(mode="numeric")

# "TBT TOPOLOGICAL BINARY TEST OUTPUT"
#Input of TBT is a set of integers (16 bits) will be converted to base 2 so to a bit sequence
#Tested an has no problem whatever the payload length is
alpha = 0.05
i <- 1
j <- 1
# "LENGTH OF HEX STRING"
# print(nchar(y))
# if (nchar(y) < 100) {
#    return (alpha)
# }
if(nchar(y) < 800){
  B=8  
while (i < nchar(y)) {
  x <- append(x, hex2dec(substr(y, i, i+1)))

  i <- i + 2
}

x=sfsmisc::digitsBase(x, base= 2, B) #Convert to base 2

critical.value= 2311           #Obtained for B = 8
} else if (nchar(y) < 1200){
    B=12  
while (i < nchar(y)-1) {
  x <- append(x, hex2dec(substr(y, i, i+2)))

  i <- i + 3
}

x=sfsmisc::digitsBase(x, base= 2, B) #Convert to base 2

critical.value= 4622
} else {
     B=16  
while (i < nchar(y)-2) {
  x <- append(x, hex2dec(substr(y, i, i+3)))

  i <- i + 4
}

x=sfsmisc::digitsBase(x, base= 2, B) #Convert to base 2

critical.value= 9245
}

test=topological.binary(x, B, alpha, critical.value)
#Idea of getting p-value: since the output value has to be equal or more the critical value to pass the test
# that means if output Value = critical value => p-value = 0.05 => p-value = Value/critical.value * 20
# print(test$statistic/(critical.value*20))
#print(test)
return(test$statistic/(critical.value*20))
}