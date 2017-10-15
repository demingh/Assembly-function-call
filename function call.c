 

#define RANDOMIZE_CALLEE_REGISTERS __asm mov ebx, 0xAFAFAFAF \
                                   __asm mov esi, 0xAFAFAFAF \
                                   __asm mov edi, 0xAFAFAFAF 
#define RANDOMIZE_CALLER_REGISTERS __asm mov eax, 0xAFAFAFAF \
                                   __asm mov ecx, 0xAFAFAFAF \
                                   __asm mov edx, 0xAFAFAFAF

/***********************************************************************************************

PART 1: Change Letter Case

You are given a string (null-terminated array of characters) that may contain numbers,
upper and lower case letters, punctuations symbols, etc. Implement a function to convert all
uppercase letters to lowercase and vice versa.
For example "ThisIsTheFinalLabOf51" should be converted to "tHISiStHEfINALlABoF51".
The string should be modifed in place. There is no explicit output for this function.

Tips:

- Note that each element of the arrays is a char (1 byte). You can NOT use a 32-bit register
to read/write from/to a memory location. Instead you should use the 8-bit version of the x86
general-purpose registers.

- The link to the ASCII codes is here: http://www.asciitable.com/

************************************************************************************************/
void changeCase(char *string) {

	RANDOMIZE_CALLEE_REGISTERS
		RANDOMIZE_CALLER_REGISTERS

		__asm
	{
		// YOUR CODE STARTS HERE

		xor eax, eax;
		mov ebx, string;
	while_start:
		mov ch, byte ptr[ebx + eax]
			cmp ch, 0
			je end
			cmp ch, 65
			jl restart_loop
			cmp ch, 90
			jle changetoUpper
			cmp ch, 97
			jl restart_loop
			cmp ch, 122
			jg restart_loop
			sub ch, 32
			mov byte ptr[ebx + eax], ch
			jmp restart_loop

		changetoUpper :
		add ch, 32
			mov byte ptr[ebx + eax], ch
			jmp restart_loop

		restart_loop :
		inc eax
			jmp while_start

		end :
		// YOUR CODE ENDS HERE
	}
}

/***********************************************************************************************

PART 2: Selection Sort

Selection sort is an in-place comparison sort algorithm that works by dividing the input list
into two parts: the sublist of items already sorted, which is built up from left to right of
the list, and the sublist of items remaining to be sorted that occupy the rest of the list.
Initially, the sorted sublist is empty and the unsorted sublist is the entire input list.
The algorithm proceeds by finding the smallest element in the unsorted sublist, exchanging it
with the leftmost unsorted element (putting it in sorted order), and moving the sublist
boundaries one element to the right. To learn more, read:
https://en.wikipedia.org/wiki/Selection_sort#Example

Our implementation here uses a function called "findMinIndex" to find the index of smallest
element between ith index and jth index of the array. The function "selectionSort" uses
this function to find the smallest number and exchanges it with the leftmost unsorted element
in each iteration. You need to implement the behavior of both functions in x86 assembly.

************************************************************************************************/
__declspec(naked)
int findMinIndex(int integer_array[], int i, int j)
{

	// C code to be converted to x86 assembly
	/*
	int iMin = i;
	// test against elements after i and before j to find the smallest
	for ( i ; i < j; i++) {
	// if this element is less, then it is the new minimum
	if (integer_array[i] < integer_array[iMin]) {
	// found new minimum; remember its index
	iMin = i;
	}
	}

	return iMin;
	*/

	RANDOMIZE_CALLER_REGISTERS

		__asm
	{
		// YOUR CODE STARTS HERE
		push ebp
			mov ebp, esp
			sub esp, 4

			push edi
			push esi
			push ebx

			mov eax, [ebp + 8] //pointer of integer_array
			mov esi, [ebp + 12]//i
			mov edi, [ebp + 16]//j
			add edi, 1
			mov ebx, dword ptr[eax + esi * 4] // interer_array[i]
			mov edx, esi
		start_loop :
		cmp esi, edi
			jge end
			mov ecx, dword ptr[eax + esi * 4]
			cmp ecx, ebx
			jge next
			mov ebx, ecx
			mov edx, esi

		next :
		inc esi
			jmp start_loop
		end :
		mov eax, edx

			pop ebx
			pop esi
			pop edi

			mov esp, ebp
			pop ebp
			// YOUR CODE ENDS HERE
			ret
	}

}

void selectionSort(int integer_array[], int array_size)
{

	// C code to be converted to x86 assembly
	/*
	int j;
	int iMin;
	int temp;

	// advance the position through the entire array //
	// (could do j < n-1 because single element is also min element) //
	for (j = 0; j < array_size-1; j++) {

	// find the index of min element in the unsorted a[j .. n-1] //
	iMin = findMinIndex (integer_array, j, array_size);

	if(iMin != j) { // swap values
	temp = integer_array[iMin];
	integer_array[iMin] = integer_array [j];
	integer_array[j] = temp;
	}
	}
	*/

	RANDOMIZE_CALLEE_REGISTERS
		RANDOMIZE_CALLER_REGISTERS

		__asm
	{
		// YOUR CODE STARTS HERE
		mov eax, integer_array
			mov ebx, array_size
			xor esi, esi  //counter j
			xor edi, edi  // return variable iMin
			sub ebx, 1
		start_loop:
		cmp esi, ebx
			jge end

			push eax

			push ebx
			push esi
			push eax

			call findMinIndex
			add esp, 12

			mov edi, eax
			pop eax

			cmp edi, esi
			je next

			mov ecx, dword ptr[eax + esi * 4]
			mov edx, dword ptr[eax + edi * 4]
			mov dword ptr[eax + esi * 4], edx
			mov dword ptr[eax + edi * 4], ecx

		next :
		inc esi
			jmp start_loop

		end :


		// YOUR CODE ENDS HERE
	}

}

/***********************************************************************************************

PART 3: Palindrome

Given a string, complete the implementation of the recursive function isPalindrome() to check
if the string is palindrome or not. A string is said to be palindrome if reverse of the
string is same as string. For example, “abba” is palindrome, but “abbc” is not palindrome.

************************************************************************************************/
__declspec(naked)
int isPalindrome(char *input_string, int left_index, int right_index)
{

	// C code to be converted to x86 assembly
	/*
	// Input Validation
	if (NULL == input_string || left_index < 0 || right_index < 0){
	return -1;
	}
	// Recursion termination condition
	if (left_index >= right_index)
	return 1;
	if (input_string[left_index] == input_string[right_index]){
	return isPalindrome(input_string, left_index + 1, right_index - 1);
	}
	return -1;
	*/

	RANDOMIZE_CALLER_REGISTERS

		__asm
	{
		// YOUR CODE STARTS HERE
		push ebp
			mov ebp, esp

			push edi   //local variable for input_string pushed onto stack  
			push esi  // local variable for left_index pushed onto stack
			push ebx   // local variable for right_index pushed onto stack 

			mov edi, dword ptr[ebp + 8]  //input_string
			mov esi, dword ptr[ebp + 12]  // left_index
			mov ebx, dword ptr[ebp + 16]  //right_index 

			cmp edi, 0   //(if NULL == input_string)
			je end_neg_one
			cmp esi, 0  //(if left<0)
			jl end_neg_one
			cmp ebx, 0   //(if right<0)
			jl end_neg_one

			cmp esi, ebx // if (left >= right)
			jge end_pos_one

			mov cl, byte ptr[edi + esi]
			mov ch, byte ptr[edi + ebx]
			cmp cl, ch //if (input_string[left_index] == input_string[right_index])
			jne end_neg_one

			add esi, 1     //left+1
			sub ebx, 1      //right -1

			push ebx   //push parameters from right to left to stack   
			push esi
			push edi
			// normailly we push eax, ecx , edx, but in here we dont need to right? question 1
			call isPalindrome
			add esp, 12    //create 3 int variable space 
			jmp End

		end_neg_one :
		mov eax, -1
			jmp end

		end_pos_one :
		mov eax, 1
			jmp end

		end :
		pop ebx
			pop esi
			pop edi

			mov esp, ebp
			pop ebp

			// YOUR CODE ENDS HERE
			ret
	}
}

/***********************************************************************************************

PART 4: Vector Operation

The vectorOperations function gets as parameters 4 arrays of the same length and
the number of elements in those arrays.

For each pair of elements in first_array and second_array, it calls performOperation
and stores the returned result in the same index of output_array.

output_array[index] = performOperation(operation_array[index], first_array[index], second_array[index]).

You also need to implement the body of performOperation function according to the provided
C code.

************************************************************************************************/
__declspec(naked)
int performOperation(int operation, int first, int second)
{

	// C code to be converted to x86 assembly
	/*
	if (operation==0)
	return first - second;
	else if (operation==1)
	return first + second;
	else if (operation==2)
	return (first < second)? first : second;
	else if (operation==3)
	return (first > second)? first : second;
	else
	return first;
	*/

	RANDOMIZE_CALLER_REGISTERS

		__asm
	{
		// YOUR CODE STARTS HERE


		push ebp
			mov ebp, esp

			push edi
			push esi
			push ebx

			mov ebx, [ebp + 8]   //operation  +4 instead of +8
			mov esi, [ebp + 12]  //first
			mov edi, [ebp + 16]  //second

			cmp ebx, 0
			je first_return
			cmp ebx, 1
			je second_return
			cmp ebx, 2
			je third_return
			cmp ebx, 3
			je fourth_return
			jmp first_result


		first_return :
		sub esi, edi
			jmp first_result
		second_return :
		add esi, edi
			jmp first_result
		third_return :
		cmp esi, edi
			jge second_result
			jmp first_result

		fourth_return :
		cmp esi, edi
			jle second_result
			jmp first_result

		first_result :
		mov eax, esi
			jmp end
		second_result :
		mov eax, edi

		end :

		pop ebx
			pop esi
			pop edi

			mov esp, ebp
			pop ebp
			// YOUR CODE ENDS HERE
			ret
	}
}

void vectorOperations(int number_of_elements, int *first_array, int *second_array,
	int *operation_array, int *output_array)
{
	// C code to be converted to x86 assembly
	/*
	int i;
	for (i=0; i<number_of_elements; i++)
	{
	output_array[i] = performOperation(operation_array[i], first_array[i], second_array[i]);
	}
	*/

	RANDOMIZE_CALLEE_REGISTERS
		RANDOMIZE_CALLER_REGISTERS

		__asm
	{
		// YOUR CODE STARTS HERE


		xor ecx, ecx

		start_loop :
		mov esi, first_array
			mov edi, second_array
			mov ebx, operation_array
			mov edx, number_of_elements

			cmp ecx, edx
			jge end

			mov esi, dword ptr[esi + ecx * 4]
			mov edi, dword ptr[edi + ecx * 4]
			mov ebx, dword ptr[ebx + ecx * 4]

			push ecx
			push edx

			push edi
			push esi
			push ebx

			call performOperation
			add esp, 12

			pop edx
			pop ecx

			mov edi, output_array
			mov dword ptr[edi + ecx * 4], eax
			inc ecx
			jmp start_loop
		end :
		// YOUR CODE ENDS HERE
	}
}

/***********************************************************************************************

PART 5: Dijkstra's Algorithm to calculate GCD.

Implement a recursive function in assembly that calculates the greatest common divisor of
two unsigned integers.

Given two unsigned integers n and m, we can define GCD(m , n) as:

GCD(m , n) = n                       ,    if (m % n) == 0
GCD(m , n) = GCD(n , m % n)          ,    if (m % n) >  0

************************************************************************************************/
__declspec(naked)
unsigned int gcd(unsigned int m, unsigned int n) {

	// C code to be converted to x86 assembly
	/*
	if ((m % n) == 0)
	return n;
	else
	return gcd(n, m % n);
	*/

	RANDOMIZE_CALLER_REGISTERS

		__asm
	{
		// YOUR CODE STARTS HERE
		push ebp
			mov ebp, esp

			push edi
			push ebx

			mov eax, dword ptr[ebp + 8]    //   idiv ebx — divide the contents of EDX : EAX by the contents of EBX.Place the quotient in EAX and the remainder in EDX.
			mov ebx, dword ptr[ebp + 12]
			xor edx, edx
			idiv ebx

			cmp edx, 0
			jne else_if
			mov eax, dword ptr[ebp + 12]
			jmp end

		else_if :
		mov edi, edx
			mov ebx, dword ptr[ebp + 12]

			push edi
			push ebx

			call gcd

		end :
		pop ebx
			pop edx

			mov esp, ebp
			pop ebp
			//new End
			// YOUR CODE ENDS HERE
			ret
	}
}
