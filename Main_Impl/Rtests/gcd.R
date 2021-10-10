gcdfunc <- function(input){
import("CryptRndTest")
import(broman)
import(purrr)
possibly <- purrr::possibly
y <- input
x1 <- vector(mode="numeric")
x2 <- vector(mode="numeric")
alpha = 0.05

# " GCD TEST OUTPUT"
#Input of GCD is an array of two sets of intgers (k is number of integers in a set)
## Three cases for bit length is considered B = 4, 8, 16 and 24 which is the maximum can be considered 
## when converting from hex to dec.

## We obtained values of \code{mu} and \code{sd} for other bit settings as 
## \code{mu=4.2503, sd=1.650673} for 8-bits, \code{mu=8.8772, sd=2.38282} for 16-bits, ...for 24-bits,...
# if (nchar(y) < 100) {
#    return (alpha)
# }
if (nchar(y) < 800) {
   B=8                 # Bit length is 16. 
k= as.integer(nchar(y)/2)            # Generate 250 integers.
i <- 1
while (i < k-2) {
  x1 <- append(x1, hex2dec(substr(y, i, i+1)))

  i <- i + 2
}
i <- k
while (i < (k*2)) {
  x2 <- append(x2, hex2dec(substr(y, i, i+1)))
  if (length(x1)== length(x2)) {
     break
  }
  i <- i + 2
}
mu=4.2503
sd=1.650673
} else if (nchar(y) < 1400) {
   B=16                 # Bit length is 16. 
k= as.integer(nchar(y)/2)            # Generate 250 integers.
i <- 1
while (i < k-2) {
  x1 <- append(x1, hex2dec(substr(y, i, i+3)))

  i <- i + 4
}
i <- k
while (i < (k*2)) {
  x2 <- append(x2, hex2dec(substr(y, i, i+3)))
  if (length(x1)== length(x2)) {
     break
  }
  i <- i + 4
}
mu=8.8772
sd=2.38282
} else {
   B=24                 # Bit length is 16. 
k= as.integer(nchar(y)/2)            # Generate 250 integers.
i <- 1
while (i < k-2) {
  x1 <- append(x1, hex2dec(substr(y, i, i+5)))

  i <- i + 6
}
i <- k
while (i < (k*2)) {
  x2 <- append(x2, hex2dec(substr(y, i, i+5)))
  if (length(x1)== length(x2)) {
     break
  }
  i <- i + 6
}
mu=11.8772  # need to be checked from documentation, evaluated by me
sd=3.38282
}

#print(x1)
# "Print K x1 and x2"
# print(k)
# print(length(x1))
# print(length(x2))
x=array(0,dim=c(length(x1),2))
x[,1] <- x1
x[,2] <- x2
# print(x)
## if there are one zero integer of the converted hex the test can not be applied in the library and
## an error will occur so we decided a default value in this case as a gcd test output which is the alph value
if (sum(x==0) > 0){ 
    return (alpha)
} else {

posslm2 = possibly(.f = GCD.test, otherwise = "NULL")

sink("file")
test= posslm2(x,B=B,KS=TRUE,CSQ=TRUE,AD=TRUE,JB=TRUE,
              test.k=TRUE,test.g=TRUE,mu=mu,sd=sd,alpha=alpha)
sink()
if (test == "NULL") {
   return (alpha)
}
else {
   return(test$CSQ.result.k)
}

# test=GCD.test(x,B=B,KS=TRUE,CSQ=TRUE,AD=TRUE,JB=TRUE,
#               test.k=TRUE,test.g=TRUE,mu=mu,sd=sd,alpha=alpha)
#print(test)
## In gcd test there are multiple p-values calculated for different goodnis of fit distributions.. 
## Hence, we decided to get the mean of all values  as one output p-value of gcd test.
# sumv <- c(test$sig.value.k, test$sig.value.g)
## print(test$sig.value.k)
## class(test$sig.value.k)
## print(test$KS.result.k)
## print(test$CSQ.result.k) ### this will be the one 
## print(test$AD.result.k)
## print(test$JB.result.k)
## print(test$test.k)
## print(test$test.g)
## #class(x)
## print(test$sig.value.g)
# print(sumv)
# ## class(sumv)
# print(mean(sumv))

# return (test$CSQ.result.k)
}
}