
namespace sourcesdk::utl {

template <typename T>
class Memory {
   public:
    T* mem;
    int count;

   private:
    int grow;
};

}  // namespace sourcesdk::utl
