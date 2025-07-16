#include "types.h"
#include "stat.h"
#include "user.h"

// M1: getpinfo
struct pinfo {
  int pid[64];
  int mem[64];
  char name[64][16];
};

void test_getpinfo() {
  struct pinfo p;
  getpinfo(&p);

  printf(1, "== Info Proses Aktif ==\n");
  printf(1, "PID\tMEM\tNAME\n");
  for (int i = 0; i < 64 && p.pid[i]; i++) {
    printf(1, "%d\t%d\t%s\n", p.pid[i], p.mem[i], p.name[i]);
  }
}

// M2: priority scheduling
void busy() {
  for (volatile int i = 0; i < 100000000; i++);
}

void test_priority() {
  int pid1 = fork();
  if (pid1 == 0) {
    set_priority(90); // prioritas rendah
    sleep(1);  // 🔁 Delay sedikit agar Child 2 bisa masuk lebih dulu
    busy();
    printf(1, "Child 1 selesai\n");
    exit();
  }

  int pid2 = fork();
  if (pid2 == 0) {
    set_priority(10); // prioritas tinggi
    busy();
    printf(1, "Child 2 selesai\n");
    exit();
  }

  wait(); wait();
  printf(1, "Parent selesai\n");
}


int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf(1, "Usage: ptest [modul1|modul2]\n");
    exit();
  }

  if (strcmp(argv[1], "modul1") == 0) {
    test_getpinfo();
  } else if (strcmp(argv[1], "modul2") == 0) {
    test_priority();
  } else {
    printf(1, "Argumen tidak valid. Gunakan: modul1 atau modul2\n");
  }

  exit();
}
