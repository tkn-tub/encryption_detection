bookstackfunc <- function(input){
import("CryptRndTest")
import(broman)
#install.packages("purrr")
import(purrr)
possibly <- purrr::possibly
y <- input
xb <- vector(mode="numeric")
## three cases for bit length is considered B = 8, 16 and 24 
##(Tried B=4 but always getting error when length is changing!!) 24 is the maximum can be considered 
## when converting from hex to dec.
# " Book Stack TEST OUTPUT"
# print(nchar(y))
i <- 1
alpha = 0.05
i <- 1
j <- 1
# "LENGTH OF HEX STRING"
# print(nchar(y))
# if (nchar(y) < 300) { ## can be moved to c++
#    return (alpha)
# }
## After testing on different lengths of hexString(input) we decided to make the length of string under
## the below condition to avoid errors from original library.
## further testing might be needed
while ((nchar(y)%%4) != 0) {
   y<-sub(".", "", y)
}
# print(nchar(y))
if (nchar(y) < 256) {
   B=8                 # Bit length is 8. 
while (i < (nchar(y))) {
  xb <- append(xb, hex2dec(substr(y, i, i+1)))

  i <- i + 2
    }
} else 
if(nchar(y) < 512){
B=16                 # Bit length is 8. 

while (i < (nchar(y))-2) {
  xb <- append(xb, hex2dec(substr(y, i, i+3)))

  i <- i + 4
    }
}  else {
B=24                 # Bit length is 8. 

while (i < (nchar(y))-4) {
  xb <- append(xb, hex2dec(substr(y, i, i+5)))

  i <- i + 6
    }
}

k=2                   # Divide alphabet to two sub-sets.
posslm2 = possibly(.f = book.stack, otherwise = "NULL")
#use sink as workaround to hide printed output
#there was undesired output (burada from book.stack.main.R)
sink("file")
test= posslm2(xb, B, k, alpha, bit = FALSE)
sink()
# invisible(capture.output(test = posslm2(xb, B, k, alpha, bit = FALSE)))
# print(1/test$statistic)
# print(test$p.value)
if (test == "NULL") {
   return (alpha)
}
else {
   return(test$p.value)
}
}