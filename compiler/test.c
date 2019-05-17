int fattoriale(int n) {
  int i;
  int risultato = 1;
  for (i = 2; i <= n; i = i + 1) {
    risultato = risultato * i;
  }
  return risultato;
}

int main() {
  int r = fattoriale(5);
  print(r);
  return 0;
}

int fn() {
  int a = 3;
  int b = 2;

  if (a == 2) {
    return 5;
  } else {
    return 2;
  }

  for (b = 0; b < 5; b = b + 1) {
    a = a * 2;
  }

  while (a == 0) {
    a = b + 4 - fn(3 + a);
  }

  return a;
}
