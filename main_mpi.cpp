#include<boost/mpi.hpp>

int main(int argc,char **argv){
    boost::mpi::environment env(argc,argv);
    boost::mpi::communicator world;
    boost::mpi::communicator newcomm = world.split(world.rank()/2,world.rank());
    std::cout<<"rank: "<<world.rank()<<" size: "<<world.size()<<std::endl;
    std::cout<<"newcomm rank: "<<newcomm.rank()<<" size: "<<newcomm.size()<<std::endl;
    std::cout<<"Hello World! I am process "<<world.rank()<<" of "<<world.size()<<"."<<std::endl;
    
    newcomm.send()
    
    return 0;
}