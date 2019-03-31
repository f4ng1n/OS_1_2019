# OS2019
######ETU"LETI" project (OS 2019)#######

***LAB SỐ 1: СОЗДАНИЕ И УНИЧТОЖЕНИЕ ПОТОКОВ
(TẠO VÀ XÂY DỰNG LUỒNG _THREAD)***

*Цель работы - знакомство с базовой структурой построения многопоточной программы и с системными вызовами, обеспечивающими создание и завершение потоков. (Mục đích của bài lab: làm quen với cấu trúc cơ bản, nguyên lý và biết cách xây dựng một chương trình đa luồng, bằng cách gọi luồng hệ thống, đảm bảo thực hiện và dừng luồng).*
*Tổng hợp lý thuyết từ lection conspectus (конспект лекции у Широкова)*

**1. В чем состоит различие между понятиями «поток» и «процесс»?**
(Sự khác biệt về định nghĩa giữa “luồng” và “tiến trình” là gì?)
  Процессы и потоки связаны друг с другом, но при этом имеют существенные различия.
Процесс — экземпляр программы во время выполнения, независимый объект, которому выделены системные ресурсы (например, процессорное время и память). Каждый процесс выполняется в отдельном адресном пространстве: один процесс не может получить доступ к переменным и структурам данных другого. Если процесс хочет получить доступ к чужим ресурсам, необходимо использовать межпроцессное взаимодействие. Это могут быть конвейеры, файлы, каналы связи между компьютерами и многое другое.
Поток использует то же самое пространства стека, что и процесс, а множество потоков совместно используют данные своих состояний. Как правило, каждый поток может работать (читать и писать) с одной и той же областью памяти, в отличие от процессов, которые не могут просто так получить доступ к памяти другого процесса. У каждого потока есть собственные регистры и собственный стек, но другие потоки могут их использовать.
Поток — определенный способ выполнения процесса. Когда один поток изменяет ресурс процесса, это изменение сразу же становится видно другим потокам этого процесса.
Process là quá trình hoạt động của một ứng dụng, một đối tượng độc lập cho tài nguyên của hệ thống (ví dụ thời gian xử lý và bộ nhớ). Mỗi một tiến trình (process) thực hiện trong một không gian địa chỉ riêng biệt: 1 process không thể truy cập vào các biến cũng như cấu trúc dữ liệu của một process khác. Nếu muốn truy cập, cần phải có sự giao tiếp giữa các process với nhau. Sự giao tiếp này có thể là pipelines, files communication channels giữa các máy tính và hơn thế nữa.
Thread (luồng) sử dụng chung stack (ngăn xếp) của process, và multiple threads (đa luồng) chia sẻ dữ liệu trạng thái của chúng. Theo nguyên tắc, mỗi luồng có thể đọc và ghi (read and write) với cùng 1 vùng nhớ. Điều này khác biệt với process, không thể dù chỉ đơn giản là truy cập vào vùng nhớ của tiến trình khác. Mỗi luồng có một thanh ghi riêng và ngăn xếp riêng, nhưng các luồng khác có thể sử dụng chúng.
 Luồng – phương thức xác định thực hiện các tiến trình! Khi 1 luồng thay đổi tài nguyên của tiến trình, sự thay đổi này thực sự hiển thị đối với các luồng khác của tiến trình.
 
**2. Как осуществить передачу параметров в функцию потока при создании потока? (Làm thế nào để truyền tham số vào hàm khi tạo luồng?)**
pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start)(void *), void *arg);
Аргумент arg — это бестиповый указатель, содержащий аргументы потока. Чаще всего arg указывает на глобальную или динамическую переменную, но если вызываемая функция не требует наличия аргументов, то в качестве arg можно указать NULL.
Đối số arg: con trỏ không kiểu, chứa các đối số của thread. Thường thì arg sẽ trỏ đến một global variable (biến toàn cục) hoặc biến động (dynamic variable), nhưng nếu hàm được gọi không yêu cầu các đối số thì có thể chỉ định NULL (con trỏ NULL).

**3. Какие способы завершения потока существуют? (Có các cách nào để dừng 1 luồng không?)**
Завершение работы потока может быть выполнено несколькими способами.
1.	Вызовом оператора return из функции потока; (gọi toán tử return từ hàm thread)
2.	Вызовом функции: (Gọi hàm cancel từ thread khác.)
int pthread_cancel(pthread_t thread);
из другого потока;
3.	Вызовом функции: (Gọi hàm exit)
int pthread_exit(void *value_ptr).
В последнем случае появляется возможность через переменную value_ptr передать в основной поток “код завершения”.  (Trong trường hợp cuối có thể thông qua biến value_ptr truyền vào thread “mã dừng”.
При этом необходимо синхронизировать завершение с основным потоком, используя следующую функцию: (Để làm điều này cần đồng bộ hóa việc dừng với luồng chính, sử dụng hàm sau _ hàm join)
int pthread_join(pthread_t thread, void ** retval),
где:
thread – идентификатор потока; // nhận diện luồng (id thread)
retval – код завершения потока, переданный через функцию pthread_exit. // mã dừng luồng, được truyền thông qua hàm pthread_exit.
Функция, выполняющая роль потока, создается на основе следующего шаблона: // Hàm được thực hiện mục tiêu của luồng, được tạo ra dưới dạng sau
static void * thread_start(void *arg).

**4. На какие характеристики потока можно влиять через атрибуты? (Những đặc điểm nào của luồng có thể ảnh hưởng thông qua các thuộc tính)**
Атрибуты являются способом определить поведение потока, отличное от поведения по умолчанию. При создании потока с помощью pthread_create() или при инициализации переменной синхронизации может быть определен собственный объект атрибутов. Атрибуты определяются только во время создания потока; они не могут быть изменены в процессе использования.
(Các thuộc tính là một cách để xác định một hành vi của thread, khác với hành vi mặc định. Để tạo luồng bằng pthread_create() hoặc khởi tạo biến đồng bộ có thể được xác định thuộc tính của riêng mình. Các thuộc tính chỉ được xác định trong quá trình tạo luồng. Chúng không thể thay đổi trong quá trình sử dụng).
Обычно вызываются три функции: // 3 hàm thường sử dụng:
* инициализация атрибутов потока - pthread_attr_init() - создает объект pthread_attr_t tattr по умолчанию; // Khởi tạo các thuộc tính của luồng pthread_attr_init()
* изменение значений атрибутов (если значения по умолчанию не подходят) - разнообразные функции pthread_attr_*(), позволяющие установить значения индивидуальных атрибутов для структуры pthread_attr_t tattr; // Thay đổi giá trị của thuộc tính (nếu giá trị mặc định không phù hợp) – các hàm pthread_attri_*() khác nhau cho phép đặt các giá trị thuộc tính riêng cho cấu trúc tthr pthread_attr_t.
* создание потока вызовом pthread_create() с соответствующими значениями атрибутов в структуре pthread_attr_t tattr. // Tạo một luồng bằng cách gọi pthread_create() với các giá trị thuộc tính tương ứng trong cấu trúc tthr pthread_attr_t.
Функция pthread_attr_destroy() используется, чтобы удалить память для атрибутов, выделенную во время инициализации. Объект атрибутов становится недействительным. //Hàm pthread_attr_destroy() được sử dụng để xóa bộ nhớ (giải phóng bộ nhớ) phân bổ cho các thuộc tính trong quá trình khởi tạo. Đối tượng của thuộc tính trở nên không hợp lệ.

**5. В каких состояниях может находиться поток? (trong các trạng thái nào có thể chứa thread)**

Потоки могут находиться в одном из нескольких состояний:
 // Threads có thể được đặt ở một trong một vài trạng thái.
* Ready (готов) – находящийся в пуле (pool) потоков, ожидающих выполнения; 
// Ready -  tồn tại trong pool của thread, chờ để thi hành.
* Running (выполнение) - выполняющийся на процессоре;
 // Running -  thực thi trong bộ xử lý.
* Waiting (ожидание), также называется idle или suspended, приостановленный - в состоянии ожидания, которое завершается тем, что поток начинает выполняться (состояние Running) или переходит в состояние Ready; 
// Waiting, cũng được gọi là idle hoặc suspended, (treo), ở trạng thái chờ, kết thúc bằng chuỗi bắt đầu thực thi hoặc là chuyển sang trạng thái Ready.
* Terminated (завершение) - завершено выполнение всех команд потока. Впоследствии его можно удалить. Если поток не удален, система может вновь установить его в исходное состояние для последующего использования.
 // Terminated – thực hiện tất cả các lệnh trong thread đã được hoàn thành. Có thể xóa chúng sau đó. Nếu thread không bị xóa, hệ thống có thể đặt lại về trạng thái ban đầu để sử dụng sau.
6. <leksi>
**7.  Объясните суть параметров, входящих в вызов pthread_create(). (Giải thích con đường của các đối số, đi vào từ gọi hàm pthread_create()** 
Новый поток создается функцией pthread_create(), объявленной в заголовочном файле pthread.h. 
//Thread mới được tạo ra từ hàm pthread_create(), được khai báo trong header pthread.h. (thư viện).
При удачном завершении pthread_create() возвращает код 0, ненулевое значение сигнализирует об ошибке.
	// Khi hàm thực thi thành công pthread_create(), trả về giá trị 0, giá trị khác 0 sẽ dẫn tới báo lỗi.
pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start)(void *), void *arg);
* Первый параметр вызова pthread_create() является адресом для хранения идентификатора создаваемого потока типа pthread_t.
	//Đối số đầu tiên của cuộc gọi pthread_create() gọi là địa chỉ để lưu trữ id của thread đã tạo, kiểu pthread_t.
* Аргумент start является указателем на потоковую void * функцию, принимающей бестиповый указатель в качестве единственной переменной.
	// Đối số start gọi là con trỏ đến luồng hàm void,  hàm này chấp nhận con trỏ không kiểu là biến duy nhất.
* Аргумент arg — это бестиповый указатель, содержащий аргументы потока. Чаще всего arg указывает на глобальную или динамическую переменную, но если вызываемая функция не требует наличия аргументов, то в качестве arg можно указать NULL.
 	// Đối số arg – là 1 con trỏ không kiểu, chứa tham số của luồng. Thông thường arg trỏ đến biến toàn cục hoặc là biến động, nhưng nếu hàm đc gọi không yêu cầu đối số cụ thể, thì con trỏ có thể là con trỏ NULL.
* Аргумент attr также является бестиповым указателем атрибутов потока pthread_attr_t. Если этот аргумент равен NULL, то поток создается с атрибутами по умолчанию.
 	// Đối số attr cũng là một con trỏ không kiểu của thuộc tính luồng pthread_attr_t. Nếu như đối số này bằng NULL, thì thread được tạo ra với thuộc tính mặc định.
