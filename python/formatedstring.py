
a = "INSERT INTO dhtdata (MAC, IP, temperature, humidity, systime) VALUES ('%s', '%s', %3.2f, %3.1f, '%s');"
print(a)
b = a % ('999888777666','77.77.88.99',56.234,84.1234,'20991212010101')
print(b)
