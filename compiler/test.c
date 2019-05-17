int fattoriale(int n) {
  int i;
  int risultato = 1;
  for (i = 2; i < n; i = i + 1) {
    risultato = risultato * i;
  }
  risultato = risultato * i;
  return risultato;
}

int main() {
  int r = fattoriale(5);
  print(r);
  return 0;
}
