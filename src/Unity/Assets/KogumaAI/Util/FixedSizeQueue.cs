using System.Collections;
using System.Collections.Generic;

public class FixedSizedQueue<T> : Queue<T>
{
    public int Limit { get; set; }
    public T lastestItem;

    public FixedSizedQueue(int Limit) :base(Limit){
        this.Limit = Limit;
    }
    public void Enqueue(T obj)
    {
        this.lastestItem = obj;
        if (base.Count == this.Limit)
        {
            base.Dequeue();
        }
        base.Enqueue(obj);
    }
    public T Dequeue(){
        return base.Dequeue();
    }
}
