#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

volatile int shared = 0;

void
worker(void *a, void *b)
{
  int x = (int)a;
  int y = (int)b;
  shared = x + y;
  exit();
}

int
main(void)
{
  void *stack1;
  void *stack2;
  void *joined_stack;
  int pid;

  // Start banner to make test output easy to identify in xv6 console logs.
  printf(1, "kthreadtest: start\n");

  // Allocate one page for thread #1 stack; clone expects a page-aligned stack.
  stack1 = sbrk(PGSIZE);
  if(stack1 == (void*)-1){
    printf(1, "kthreadtest: sbrk failed for stack1\n");
    exit();
  }

  // Spawn first thread; worker should write 7 + 5 into shared.
  pid = clone(worker, (void*)7, (void*)5, stack1);
  if(pid < 0){
    printf(1, "kthreadtest: clone #1 failed\n");
    exit();
  }

  // Reap first thread and receive its stack pointer from join.
  if(join(&joined_stack) < 0){
    printf(1, "kthreadtest: join #1 failed\n");
    exit();
  }

  // Parent should observe child updates because clone shares address space.
  printf(1, "kthreadtest: shared after thread #1 = %d\n", shared);
  // join should return the same user stack pointer passed to clone.
  printf(1, "kthreadtest: join stack #1 %s\n",
         joined_stack == stack1 ? "matches" : "does-not-match");

  // Repeat with a second stack and different args to confirm stable behavior.
  stack2 = sbrk(PGSIZE);
  if(stack2 == (void*)-1){
    printf(1, "kthreadtest: sbrk failed for stack2\n");
    exit();
  }

  pid = clone(worker, (void*)20, (void*)22, stack2);
  if(pid < 0){
    printf(1, "kthreadtest: clone #2 failed\n");
    exit();
  }

  if(join(&joined_stack) < 0){
    printf(1, "kthreadtest: join #2 failed\n");
    exit();
  }

  printf(1, "kthreadtest: shared after thread #2 = %d\n", shared);
  printf(1, "kthreadtest: join stack #2 %s\n",
         joined_stack == stack2 ? "matches" : "does-not-match");

  // Negative test: unaligned stack should be rejected by clone.
  pid = clone(worker, (void*)1, (void*)2, (void*)((char*)stack2 + 1));
  if(pid == -1)
    printf(1, "kthreadtest: unaligned-stack clone rejected (expected)\n");
  else
    printf(1, "kthreadtest: unaligned-stack clone accepted (unexpected)\n");

  // Final verdict for quick pass/fail grading.
  if(shared == 42)
    printf(1, "kthreadtest: PASS\n");
  else
    printf(1, "kthreadtest: FAIL (shared=%d)\n", shared);

  exit();
}
