int getDepth(Node *t){

  if (t == NULL) {

    return 0;

  }

  return 1 + max(getDepth(t->left),getDepth(t->right));

}



int max(int a ,int b){

  return a>b?a:b;

}